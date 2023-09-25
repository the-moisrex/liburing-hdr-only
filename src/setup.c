/* SPDX-License-Identifier: MIT */
#define _DEFAULT_SOURCE

#include "lib.h"
#include "syscall.h"
#include "../src/liburing.h"
#include "liburing/compat.h"
#include "liburing/io_uring.h"


__cold struct io_uring_probe *io_uring_get_probe(void) {
    struct io_uring ring;
    struct io_uring_probe *probe;
    int r;

    r = io_uring_queue_init(2, &ring, 0);
    if (r < 0)
        return nullptr;

    probe = io_uring_get_probe_ring(&ring);
    io_uring_queue_exit(&ring);
    return probe;
}


static size_t npages(size_t size, long page_size) {
    size--;
    size /= page_size;
    return internal__fls(uring_static_cast(int, size));
}

#define KRING_SIZE    320

static size_t rings_size(struct io_uring_params *p, unsigned entries,
                         unsigned cq_entries, long page_size) {
    size_t pages, sq_size, cq_size;

    cq_size = sizeof(struct io_uring_cqe);
    if (p->flags & IORING_SETUP_CQE32)
        cq_size += sizeof(struct io_uring_cqe);
    cq_size *= cq_entries;
    cq_size += KRING_SIZE;
    cq_size = (cq_size + 63) & ~63UL;
    pages = (size_t) 1 << npages(cq_size, page_size);

    sq_size = sizeof(struct io_uring_sqe);
    if (p->flags & IORING_SETUP_SQE128)
        sq_size += 64;
    sq_size *= entries;
    pages += (size_t) 1 << npages(sq_size, page_size);
    return pages * page_size;
}

/*
 * Return the required ulimit -l memlock memory required for a given ring
 * setup, in bytes. May return -errno on error. On newer (5.12+) kernels,
 * io_uring no longer requires any memlock memory, and hence this function
 * will return 0 for that case. On older (5.11 and prior) kernels, this will
 * return the required memory so that the caller can ensure that enough space
 * is available before setting up a ring with the specified parameters.
 */
__cold ssize_t io_uring_mlock_size_params(unsigned entries,
                                          struct io_uring_params *p) {
    struct io_uring_params lp;
    struct io_uring ring;
    unsigned cq_entries, sq;
    long page_size;
    ssize_t ret;
    int cret;

    memset(&lp, 0, sizeof(lp));

    /*
     * We only really use this inited ring to see if the kernel is newer
     * or not. Newer kernels don't require memlocked memory. If we fail,
     * it's most likely because it's an older kernel and we have no
     * available memlock space. Just continue on, lp.features will still
     * be zeroed at this point and we'll do the right thing.
     */
    ret = io_uring_queue_init_params(entries, &ring, &lp);
    if (!ret)
        io_uring_queue_exit(&ring);

    /*
     * Native workers imply using cgroup memory accounting, and hence no
     * memlock memory is needed for the ring allocations.
     */
    if (lp.features & IORING_FEAT_NATIVE_WORKERS)
        return 0;

    if (!entries)
        return -EINVAL;
    if (entries > KERN_MAX_ENTRIES) {
        if (!(p->flags & IORING_SETUP_CLAMP))
            return -EINVAL;
        entries = KERN_MAX_ENTRIES;
    }

    cret = get_sq_cq_entries(entries, p, &sq, &cq_entries);
    if (cret)
        return cret;

    page_size = get_page_size();
    return rings_size(p, sq, cq_entries, page_size);
}

/*
 * Return required ulimit -l memory space for a given ring setup. See
 * @io_uring_mlock_size_params().
 */
__cold ssize_t io_uring_mlock_size(unsigned entries, unsigned flags) {
    struct io_uring_params p;

    memset(&p, 0, sizeof(p));
    p.flags = flags;
    return io_uring_mlock_size_params(entries, &p);
}

#if defined(__hppa__)
static struct io_uring_buf_ring *br_setup(struct io_uring *ring,
                      unsigned int nentries, int bgid,
                      unsigned int flags, int *ret)
{
    struct io_uring_buf_ring *br;
    struct io_uring_buf_reg reg;
    size_t ring_size;
    off_t off;
    int lret;

    memset(&reg, 0, sizeof(reg));
    reg.ring_entries = nentries;
    reg.bgid = bgid;
    reg.flags = IOU_PBUF_RING_MMAP;

    *ret = 0;
    lret = io_uring_register_buf_ring(ring, &reg, flags);
    if (lret) {
        *ret = lret;
        return NULL;
    }

    off = IORING_OFF_PBUF_RING | (unsigned long long) bgid << IORING_OFF_PBUF_SHIFT;
    ring_size = nentries * sizeof(struct io_uring_buf);
    br = __sys_mmap(NULL, ring_size, PROT_READ | PROT_WRITE,
            MAP_SHARED | MAP_POPULATE, ring->ring_fd, off);
    if (IS_ERR(br)) {
        *ret = PTR_ERR(br);
        return NULL;
    }

    return br;
}
#else

static struct io_uring_buf_ring *br_setup(struct io_uring *ring,
                                          unsigned int nentries, int bgid,
                                          unsigned int flags, int *ret) {
    struct io_uring_buf_ring *br;
    struct io_uring_buf_reg reg;
    size_t ring_size;
    int lret;

    memset(&reg, 0, sizeof(reg));
    ring_size = nentries * sizeof(struct io_uring_buf);
    br = __sys_mmap(NULL, ring_size, PROT_READ | PROT_WRITE,
                    MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (IS_ERR(br)) {
        *ret = PTR_ERR(br);
        return NULL;
    }

    reg.ring_addr = (unsigned long) (uintptr_t) br;
    reg.ring_entries = nentries;
    reg.bgid = bgid;

    *ret = 0;
    lret = io_uring_register_buf_ring(ring, &reg, flags);
    if (lret) {
        __sys_munmap(br, ring_size);
        *ret = lret;
        br = NULL;
    }

    return br;
}

#endif

struct io_uring_buf_ring *io_uring_setup_buf_ring(struct io_uring *ring,
                                                  unsigned int nentries,
                                                  int bgid, unsigned int flags,
                                                  int *ret) {
    struct io_uring_buf_ring *br;

    br = br_setup(ring, nentries, bgid, flags, ret);
    if (br)
        io_uring_buf_ring_init(br);

    return br;
}

int io_uring_free_buf_ring(struct io_uring *ring, struct io_uring_buf_ring *br,
                           unsigned int nentries, int bgid) {
    int ret;

    ret = io_uring_unregister_buf_ring(ring, bgid);
    if (ret)
        return ret;

    __sys_munmap(br, nentries * sizeof(struct io_uring_buf));
    return 0;
}
