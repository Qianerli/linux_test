cmd_/mnt/hgfs/linux_test/srio_platform/srio_drv.mod.o := /opt/sparc-gaisler-linux4.9/bin/sparc-gaisler-linux4.9-gcc -Wp,-MD,/mnt/hgfs/linux_test/srio_platform/.srio_drv.mod.o.d  -nostdinc -isystem /opt/sparc-gaisler-linux4.9/lib/gcc/sparc-gaisler-linux4.9/4.9.4/include -I/opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include -I./arch/sparc/include/generated/uapi -I./arch/sparc/include/generated  -I/opt/linuxbuild-2.0.0/linux/linux-src/include -I./include -I/opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/uapi -I/opt/linuxbuild-2.0.0/linux/linux-src/include/uapi -I./include/generated/uapi -include /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/kconfig.h -I/opt/linuxbuild-2.0.0/linux/linux-src//mnt/hgfs/linux_test/srio_platform -I/mnt/hgfs/linux_test/srio_platform -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -std=gnu89 -fno-PIE -m32 -mcpu=v8 -pipe -mno-fpu -fcall-used-g5 -fcall-used-g7 -Wa,-Av8 -fno-delete-null-pointer-checks -O2 --param=allow-store-data-races=0 -DCC_HAVE_ASM_GOTO -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -fno-var-tracking-assignments -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -Werror=implicit-int -Werror=strict-prototypes -Werror=date-time  -DKBUILD_BASENAME='"srio_drv.mod"'  -DKBUILD_MODNAME='"srio_drv"' -DMODULE  -c -o /mnt/hgfs/linux_test/srio_platform/srio_drv.mod.o /mnt/hgfs/linux_test/srio_platform/srio_drv.mod.c

source_/mnt/hgfs/linux_test/srio_platform/srio_drv.mod.o := /mnt/hgfs/linux_test/srio_platform/srio_drv.mod.c

deps_/mnt/hgfs/linux_test/srio_platform/srio_drv.mod.o := \
    $(wildcard include/config/module/unload.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/module.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/modules/tree/lookup.h) \
    $(wildcard include/config/livepatch.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/module/sig.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/debug/set/module/ronx.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/types.h \
    $(wildcard include/config/have/uid16.h) \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/types.h \
  arch/sparc/include/generated/asm/types.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/asm-generic/types.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/int-ll64.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/asm-generic/int-ll64.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/uapi/asm/bitsperlong.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitsperlong.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/asm-generic/bitsperlong.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/posix_types.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/stddef.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/stddef.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/kasan.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
    $(wildcard include/config/gcov/kernel.h) \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/uapi/asm/posix_types.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/asm-generic/posix_types.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
    $(wildcard include/config/page/poisoning/zero.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/const.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/panic/timeout.h) \
  /opt/sparc-gaisler-linux4.9/lib/gcc/sparc-gaisler-linux4.9/4.9.4/include/stdarg.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/linkage.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/stringify.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/trim/unused/ksyms.h) \
  arch/sparc/include/generated/asm/linkage.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/linkage.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/bitops.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/bitops.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/bitops_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/uapi/asm/byteorder.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/byteorder/big_endian.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/byteorder/big_endian.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/swab.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/swab.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/uapi/asm/swab.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/uapi/asm/asi.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/byteorder/generic.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/non-atomic.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/ffz.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/__ffs.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/sched.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/ffs.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/fls.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/__fls.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/fls64.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/hweight.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/arch_hweight.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/const_hweight.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/lock.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/le.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bitops/ext2-atomic.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/typecheck.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/printk.h \
    $(wildcard include/config/message/loglevel/default.h) \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk/nmi.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/init.h \
    $(wildcard include/config/debug/rodata.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/kern_levels.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/kernel.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/sysinfo.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/cache.h \
    $(wildcard include/config/sparc32.h) \
  arch/sparc/include/generated/asm/div64.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/div64.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/stat.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/uapi/asm/stat.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/stat.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/seqlock.h \
    $(wildcard include/config/debug/lock/alloc.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/preempt.h \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  arch/sparc/include/generated/asm/preempt.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/preempt.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/thread_info.h \
    $(wildcard include/config/thread/info/in/task.h) \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/debug/stack/usage.h) \
    $(wildcard include/config/have/arch/within/stack/frames.h) \
    $(wildcard include/config/hardened/usercopy.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/bug.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/thread_info.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/thread_info_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/ptrace.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/uapi/asm/ptrace.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/psr.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/uapi/asm/psr.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/switch_to.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/switch_to_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/smp.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/smp_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/head.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/head_32.h \
    $(wildcard include/config/kgdb.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/bitmap.h \
    $(wildcard include/config/s390.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/string.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/string.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/string_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/page.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/page_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/pfn.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/getorder.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/atomic.h \
    $(wildcard include/config/generic/atomic64.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/atomic.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/atomic_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/cmpxchg.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/cmpxchg_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/cmpxchg-local.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/irqflags.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/irqflags_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/barrier.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/barrier_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/barrier.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/atomic64.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/atomic-long.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/bottom_half.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/spinlock_types.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/spinlock_types.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/rwlock_types.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/spinlock.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/spinlock_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/processor.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/processor_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/signal.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/personality.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/personality.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/uapi/asm/signal.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/sigcontext.h \
    $(wildcard include/config/sparc64.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/uapi/asm/sigcontext.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/asm-generic/signal-defs.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/rwlock.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/uninline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/math64.h \
    $(wildcard include/config/arch/supports/int128.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/time64.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/time.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/uidgid.h \
    $(wildcard include/config/multiuser.h) \
    $(wildcard include/config/user/ns.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/highuid.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/kmod.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/gfp.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/zone/device.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/memory/isolation.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/cma.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/virtual.h) \
    $(wildcard include/config/debug/vm/pgflags.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/zsmalloc.h) \
    $(wildcard include/config/memcg.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/page/extension.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/numa/balancing.h) \
    $(wildcard include/config/deferred/struct/page/init.h) \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/have/memblock/node/map.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/wait.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/current.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/wait.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/nodemask.h \
    $(wildcard include/config/movable/node.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/page-flags-layout.h \
  include/generated/bounds.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/memory_hotplug.h \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/have/bootmem/info/node.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/notifier.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/errno.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/errno.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/uapi/asm/errno.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/asm-generic/errno-base.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/mutex/spin/on/owner.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/osq_lock.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/rwsem.h \
    $(wildcard include/config/rwsem/spin/on/owner.h) \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/err.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/rwsem-spinlock.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/srcu.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/rcupdate.h \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/rcu/trace.h) \
    $(wildcard include/config/rcu/stall/common.h) \
    $(wildcard include/config/no/hz/full.h) \
    $(wildcard include/config/rcu/nocb/cpu.h) \
    $(wildcard include/config/tasks/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/prove/rcu.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/rcu/nocb/cpu/all.h) \
    $(wildcard include/config/no/hz/full/sysidle.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/completion.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/ktime.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/jiffies.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/timex.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/timex.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/param.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/uapi/asm/param.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/asm-generic/param.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/timex.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/timex_32.h \
  include/generated/timeconst.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/timekeeping.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/rcutree.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
    $(wildcard include/config/wq/watchdog.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
    $(wildcard include/config/no/hz/common.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/topology.h \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
    $(wildcard include/config/sched/smt.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/smp.h \
    $(wildcard include/config/up/late/init.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/percpu.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/percpu_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/percpu.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/topology.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/topology_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/topology.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/sysctl.h \
    $(wildcard include/config/sysctl.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/rbtree.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/sysctl.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/elf.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/elf.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/elf_32.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/mbus.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/ross.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/viking.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/mxcc.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/arch/sparc/include/asm/pgtsrmmu.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/elf.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/uapi/linux/elf-em.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/kobject.h \
    $(wildcard include/config/uevent/helper.h) \
    $(wildcard include/config/debug/kobject/release.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/sysfs.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/kernfs.h \
    $(wildcard include/config/kernfs.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/idr.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/kobject_ns.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/kref.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/jump_label.h \
    $(wildcard include/config/jump/label.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/extable.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/rbtree_latch.h \
  arch/sparc/include/generated/asm/module.h \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/asm-generic/module.h \
    $(wildcard include/config/have/mod/arch/specific.h) \
    $(wildcard include/config/modules/use/elf/rel.h) \
    $(wildcard include/config/modules/use/elf/rela.h) \
  /opt/linuxbuild-2.0.0/linux/linux-src/include/linux/vermagic.h \
  include/generated/utsrelease.h \

/mnt/hgfs/linux_test/srio_platform/srio_drv.mod.o: $(deps_/mnt/hgfs/linux_test/srio_platform/srio_drv.mod.o)

$(deps_/mnt/hgfs/linux_test/srio_platform/srio_drv.mod.o):
