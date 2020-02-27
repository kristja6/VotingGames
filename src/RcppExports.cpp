// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// votingBanzhaf
NumericVector votingBanzhaf(const NumericVector& weights, const int quota);
RcppExport SEXP _CoopGame_votingBanzhaf(SEXP weightsSEXP, SEXP quotaSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericVector& >::type weights(weightsSEXP);
    Rcpp::traits::input_parameter< const int >::type quota(quotaSEXP);
    rcpp_result_gen = Rcpp::wrap(votingBanzhaf(weights, quota));
    return rcpp_result_gen;
END_RCPP
}
// votingShapley
NumericVector votingShapley(const IntegerVector& weights, const int quota);
RcppExport SEXP _CoopGame_votingShapley(SEXP weightsSEXP, SEXP quotaSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const IntegerVector& >::type weights(weightsSEXP);
    Rcpp::traits::input_parameter< const int >::type quota(quotaSEXP);
    rcpp_result_gen = Rcpp::wrap(votingShapley(weights, quota));
    return rcpp_result_gen;
END_RCPP
}
// votingVal
double votingVal(const IntegerVector& weights, const int quota, const IntegerVector& coal);
RcppExport SEXP _CoopGame_votingVal(SEXP weightsSEXP, SEXP quotaSEXP, SEXP coalSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const IntegerVector& >::type weights(weightsSEXP);
    Rcpp::traits::input_parameter< const int >::type quota(quotaSEXP);
    Rcpp::traits::input_parameter< const IntegerVector& >::type coal(coalSEXP);
    rcpp_result_gen = Rcpp::wrap(votingVal(weights, quota, coal));
    return rcpp_result_gen;
END_RCPP
}
// microarrayBanzhaf
NumericVector microarrayBanzhaf(const NumericMatrix& mtx);
RcppExport SEXP _CoopGame_microarrayBanzhaf(SEXP mtxSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericMatrix& >::type mtx(mtxSEXP);
    rcpp_result_gen = Rcpp::wrap(microarrayBanzhaf(mtx));
    return rcpp_result_gen;
END_RCPP
}
// microarrayShapley
NumericVector microarrayShapley(const NumericMatrix& mtx);
RcppExport SEXP _CoopGame_microarrayShapley(SEXP mtxSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericMatrix& >::type mtx(mtxSEXP);
    rcpp_result_gen = Rcpp::wrap(microarrayShapley(mtx));
    return rcpp_result_gen;
END_RCPP
}
// microarrayValue
double microarrayValue(const NumericMatrix& mtx, const IntegerVector& coal);
RcppExport SEXP _CoopGame_microarrayValue(SEXP mtxSEXP, SEXP coalSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericMatrix& >::type mtx(mtxSEXP);
    Rcpp::traits::input_parameter< const IntegerVector& >::type coal(coalSEXP);
    rcpp_result_gen = Rcpp::wrap(microarrayValue(mtx, coal));
    return rcpp_result_gen;
END_RCPP
}
// microarrayStrongestCoalition
NumericVector microarrayStrongestCoalition(const NumericMatrix& mtx, const int size);
RcppExport SEXP _CoopGame_microarrayStrongestCoalition(SEXP mtxSEXP, SEXP sizeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericMatrix& >::type mtx(mtxSEXP);
    Rcpp::traits::input_parameter< const int >::type size(sizeSEXP);
    rcpp_result_gen = Rcpp::wrap(microarrayStrongestCoalition(mtx, size));
    return rcpp_result_gen;
END_RCPP
}
// expressionsToFeaturesStd
NumericMatrix expressionsToFeaturesStd(const NumericMatrix& special, const NumericMatrix& control);
RcppExport SEXP _CoopGame_expressionsToFeaturesStd(SEXP specialSEXP, SEXP controlSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericMatrix& >::type special(specialSEXP);
    Rcpp::traits::input_parameter< const NumericMatrix& >::type control(controlSEXP);
    rcpp_result_gen = Rcpp::wrap(expressionsToFeaturesStd(special, control));
    return rcpp_result_gen;
END_RCPP
}
// votingMicroarrayBanzhaf
NumericVector votingMicroarrayBanzhaf(const NumericMatrix& special, const NumericMatrix& control);
RcppExport SEXP _CoopGame_votingMicroarrayBanzhaf(SEXP specialSEXP, SEXP controlSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericMatrix& >::type special(specialSEXP);
    Rcpp::traits::input_parameter< const NumericMatrix& >::type control(controlSEXP);
    rcpp_result_gen = Rcpp::wrap(votingMicroarrayBanzhaf(special, control));
    return rcpp_result_gen;
END_RCPP
}
// votingMicroarrayBanzhafTop
NumericVector votingMicroarrayBanzhafTop(const NumericMatrix& special, const NumericMatrix& control, int topN);
RcppExport SEXP _CoopGame_votingMicroarrayBanzhafTop(SEXP specialSEXP, SEXP controlSEXP, SEXP topNSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericMatrix& >::type special(specialSEXP);
    Rcpp::traits::input_parameter< const NumericMatrix& >::type control(controlSEXP);
    Rcpp::traits::input_parameter< int >::type topN(topNSEXP);
    rcpp_result_gen = Rcpp::wrap(votingMicroarrayBanzhafTop(special, control, topN));
    return rcpp_result_gen;
END_RCPP
}
// votingMicroarrayShapley
NumericVector votingMicroarrayShapley(const NumericMatrix& special, const NumericMatrix& control);
RcppExport SEXP _CoopGame_votingMicroarrayShapley(SEXP specialSEXP, SEXP controlSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericMatrix& >::type special(specialSEXP);
    Rcpp::traits::input_parameter< const NumericMatrix& >::type control(controlSEXP);
    rcpp_result_gen = Rcpp::wrap(votingMicroarrayShapley(special, control));
    return rcpp_result_gen;
END_RCPP
}
// votingMicroarrayShapleyTop
NumericVector votingMicroarrayShapleyTop(const NumericMatrix& special, const NumericMatrix& control, int topN);
RcppExport SEXP _CoopGame_votingMicroarrayShapleyTop(SEXP specialSEXP, SEXP controlSEXP, SEXP topNSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericMatrix& >::type special(specialSEXP);
    Rcpp::traits::input_parameter< const NumericMatrix& >::type control(controlSEXP);
    Rcpp::traits::input_parameter< int >::type topN(topNSEXP);
    rcpp_result_gen = Rcpp::wrap(votingMicroarrayShapleyTop(special, control, topN));
    return rcpp_result_gen;
END_RCPP
}
// votingMicroarrayValue
double votingMicroarrayValue(const NumericMatrix& special, const NumericMatrix& control, const IntegerVector& coal);
RcppExport SEXP _CoopGame_votingMicroarrayValue(SEXP specialSEXP, SEXP controlSEXP, SEXP coalSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericMatrix& >::type special(specialSEXP);
    Rcpp::traits::input_parameter< const NumericMatrix& >::type control(controlSEXP);
    Rcpp::traits::input_parameter< const IntegerVector& >::type coal(coalSEXP);
    rcpp_result_gen = Rcpp::wrap(votingMicroarrayValue(special, control, coal));
    return rcpp_result_gen;
END_RCPP
}
// votingMicroarrayPredict
bool votingMicroarrayPredict(const NumericMatrix& special, const NumericMatrix& control, const NumericVector& sample);
RcppExport SEXP _CoopGame_votingMicroarrayPredict(SEXP specialSEXP, SEXP controlSEXP, SEXP sampleSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericMatrix& >::type special(specialSEXP);
    Rcpp::traits::input_parameter< const NumericMatrix& >::type control(controlSEXP);
    Rcpp::traits::input_parameter< const NumericVector& >::type sample(sampleSEXP);
    rcpp_result_gen = Rcpp::wrap(votingMicroarrayPredict(special, control, sample));
    return rcpp_result_gen;
END_RCPP
}
// sumOfVotingBanzhaf
NumericVector sumOfVotingBanzhaf(const NumericMatrix& weights, const NumericVector& quotas);
RcppExport SEXP _CoopGame_sumOfVotingBanzhaf(SEXP weightsSEXP, SEXP quotasSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericMatrix& >::type weights(weightsSEXP);
    Rcpp::traits::input_parameter< const NumericVector& >::type quotas(quotasSEXP);
    rcpp_result_gen = Rcpp::wrap(sumOfVotingBanzhaf(weights, quotas));
    return rcpp_result_gen;
END_RCPP
}
// sumOfVotingShapley
NumericVector sumOfVotingShapley(const NumericMatrix& weights, const NumericVector& quotas);
RcppExport SEXP _CoopGame_sumOfVotingShapley(SEXP weightsSEXP, SEXP quotasSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericMatrix& >::type weights(weightsSEXP);
    Rcpp::traits::input_parameter< const NumericVector& >::type quotas(quotasSEXP);
    rcpp_result_gen = Rcpp::wrap(sumOfVotingShapley(weights, quotas));
    return rcpp_result_gen;
END_RCPP
}
// sumOfVotingValue
double sumOfVotingValue(const NumericMatrix& weights, const NumericVector& quotas, const IntegerVector& coal);
RcppExport SEXP _CoopGame_sumOfVotingValue(SEXP weightsSEXP, SEXP quotasSEXP, SEXP coalSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const NumericMatrix& >::type weights(weightsSEXP);
    Rcpp::traits::input_parameter< const NumericVector& >::type quotas(quotasSEXP);
    Rcpp::traits::input_parameter< const IntegerVector& >::type coal(coalSEXP);
    rcpp_result_gen = Rcpp::wrap(sumOfVotingValue(weights, quotas, coal));
    return rcpp_result_gen;
END_RCPP
}
// rcpp_hello_world
List rcpp_hello_world();
RcppExport SEXP _CoopGame_rcpp_hello_world() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(rcpp_hello_world());
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_CoopGame_votingBanzhaf", (DL_FUNC) &_CoopGame_votingBanzhaf, 2},
    {"_CoopGame_votingShapley", (DL_FUNC) &_CoopGame_votingShapley, 2},
    {"_CoopGame_votingVal", (DL_FUNC) &_CoopGame_votingVal, 3},
    {"_CoopGame_microarrayBanzhaf", (DL_FUNC) &_CoopGame_microarrayBanzhaf, 1},
    {"_CoopGame_microarrayShapley", (DL_FUNC) &_CoopGame_microarrayShapley, 1},
    {"_CoopGame_microarrayValue", (DL_FUNC) &_CoopGame_microarrayValue, 2},
    {"_CoopGame_microarrayStrongestCoalition", (DL_FUNC) &_CoopGame_microarrayStrongestCoalition, 2},
    {"_CoopGame_expressionsToFeaturesStd", (DL_FUNC) &_CoopGame_expressionsToFeaturesStd, 2},
    {"_CoopGame_votingMicroarrayBanzhaf", (DL_FUNC) &_CoopGame_votingMicroarrayBanzhaf, 2},
    {"_CoopGame_votingMicroarrayBanzhafTop", (DL_FUNC) &_CoopGame_votingMicroarrayBanzhafTop, 3},
    {"_CoopGame_votingMicroarrayShapley", (DL_FUNC) &_CoopGame_votingMicroarrayShapley, 2},
    {"_CoopGame_votingMicroarrayShapleyTop", (DL_FUNC) &_CoopGame_votingMicroarrayShapleyTop, 3},
    {"_CoopGame_votingMicroarrayValue", (DL_FUNC) &_CoopGame_votingMicroarrayValue, 3},
    {"_CoopGame_votingMicroarrayPredict", (DL_FUNC) &_CoopGame_votingMicroarrayPredict, 3},
    {"_CoopGame_sumOfVotingBanzhaf", (DL_FUNC) &_CoopGame_sumOfVotingBanzhaf, 2},
    {"_CoopGame_sumOfVotingShapley", (DL_FUNC) &_CoopGame_sumOfVotingShapley, 2},
    {"_CoopGame_sumOfVotingValue", (DL_FUNC) &_CoopGame_sumOfVotingValue, 3},
    {"_CoopGame_rcpp_hello_world", (DL_FUNC) &_CoopGame_rcpp_hello_world, 0},
    {NULL, NULL, 0}
};

RcppExport void R_init_CoopGame(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
