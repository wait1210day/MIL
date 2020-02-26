#ifndef __MOE_H__
#define __MOE_H__

enum class MoeTypes
{
    kMoeWarakawa,
    kMoeMashiro,
    kMoeCocoa,
    kMoeUnexpected
};

constexpr int __moe_idx_warakawa = 3769;
constexpr int __moe_idx_mashiro  = 3050;
constexpr int __moe_idx_cocoa    = 1547;

void _moe_processor(MoeTypes __type);
MoeTypes _moe_get_enumeratable_type(std::string& __str);
void _moe_warakawa();
void _moe_mashiro();
void _moe_cocoa();

#endif /* __MOE_H__ */
