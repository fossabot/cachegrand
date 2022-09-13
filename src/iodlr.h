#ifndef CACHEGRAND_IODLR_H
#define CACHEGRAND_IODLR_H

#ifdef __cplusplus
extern "C" {
#endif

bool iodlr_can_enable();
bool iodlr_try_setup(bool use_explicit_huge_pages);

#ifdef __cplusplus
}
#endif

#endif //CACHEGRAND_IODLR_H
