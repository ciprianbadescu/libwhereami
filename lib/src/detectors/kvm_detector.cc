#include <internal/detectors/kvm_detector.hpp>
#include <internal/vm.hpp>
#include <leatherman/util/regex.hpp>

using namespace leatherman::util;

namespace whereami { namespace detectors {

    static const boost::regex openstack_pattern {"^OpenStack"};
    static const boost::regex parallels_pattern {"^Parallels"};

    result kvm(const sources::cpuid_base& cpuid_source,
               sources::smbios_base& smbios_source)
    {
        result res {vm::kvm};

        // dmidecode under KVM typically reports QEMU, but CPUID does report KVM.
        // VirtualBox and Parallels will also report KVM in CPUID, though, so make sure they're not here.
        if (cpuid_source.vendor() == "KVMKVMKVM"
            && smbios_source.product_name() != "VirtualBox"
            && !re_search(smbios_source.product_name(), parallels_pattern)) {
            res.validate();

            if (smbios_source.bios_vendor() == "Google") {
                res.set("google", true);
            }

            if (re_search(smbios_source.product_name(), openstack_pattern)) {
                res.set("openstack", true);
            }
        }

        return res;
    }

}}  // namespace whereami::detectors
