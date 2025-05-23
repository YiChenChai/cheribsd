.include <bsd.own.mk>

TEST_DIR=	${SRCTOP}/contrib/byacc/test

.PATH: ${TEST_DIR} ${TEST_DIR}/yacc

PLAIN_TESTS_SH=	yacc_tests
# NOTE: due to caveats with how permissions are handled on FreeBSD
# with root, this must be run as a non-privileged user; otherwise
# the testcases will fail unexpectedly.
TEST_METADATA.yacc_tests+=	required_user="unprivileged"
# Times out on CHERI purecap QEMU
TEST_METADATA.yacc_tests+=	timeout="1800"

SCRIPTS=	run_test
SCRIPTSDIR=	${TESTSDIR}

CLEANFILES=	run_test

FILESGROUPS+=	FILEStest FILEStest_yacc
FILEStestPACKAGE=${PACKAGE}
FILEStest_yaccPACKAGE=${PACKAGE}

FILEStestDIR=	${TESTSDIR}

FILEStest+=	calc_code_all.y
FILEStest+=	calc_code_default.y
FILEStest+=	calc_code_imports.y
FILEStest+=	calc_code_provides.y
FILEStest+=	calc_code_requires.y
FILEStest+=	calc_code_top.y
FILEStest+=	calc.y
FILEStest+=	calc1.y
FILEStest+=	calc2.y
FILEStest+=	calc3.y
FILEStest+=	code_calc.y
FILEStest+=	code_debug.y
FILEStest+=	code_error.y
FILEStest+=	empty.y
FILEStest+=	err_syntax1.y
FILEStest+=	err_syntax10.y
FILEStest+=	err_syntax11.y
FILEStest+=	err_syntax12.y
FILEStest+=	err_syntax13.y
FILEStest+=	err_syntax14.y
FILEStest+=	err_syntax15.y
FILEStest+=	err_syntax16.y
FILEStest+=	err_syntax17.y
FILEStest+=	err_syntax18.y
FILEStest+=	err_syntax19.y
FILEStest+=	err_syntax2.y
FILEStest+=	err_syntax20.y
FILEStest+=	err_syntax21.y
FILEStest+=	err_syntax22.y
FILEStest+=	err_syntax23.y
FILEStest+=	err_syntax24.y
FILEStest+=	err_syntax25.y
FILEStest+=	err_syntax26.y
FILEStest+=	err_syntax27.y
FILEStest+=	err_syntax3.y
FILEStest+=	err_syntax4.y
FILEStest+=	err_syntax5.y
FILEStest+=	err_syntax6.y
FILEStest+=	err_syntax7.y
FILEStest+=	err_syntax7a.y
FILEStest+=	err_syntax7b.y
FILEStest+=	err_syntax8.y
FILEStest+=	err_syntax8a.y
FILEStest+=	err_syntax9.y
FILEStest+=	error.y
FILEStest+=	expr.oxout.y
FILEStest+=	grammar.y
FILEStest+=	ok_syntax1.y
FILEStest+=	pure_calc.y
FILEStest+=	pure_error.y
FILEStest+=	quote_calc.y
FILEStest+=	quote_calc2.y
FILEStest+=	quote_calc3.y
FILEStest+=	quote_calc4.y
FILEStest+=	varsyntax_calc1.y

FILEStest_yaccDIR=	${TESTSDIR}/yacc

FILEStest_yacc+=	big_b.error
FILEStest_yacc+=	big_b.output
FILEStest_yacc+=	big_l.error
FILEStest_yacc+=	big_l.output
FILEStest_yacc+=	calc_code_all.error
FILEStest_yacc+=	calc_code_all.output
FILEStest_yacc+=	calc_code_all.tab.c
FILEStest_yacc+=	calc_code_all.tab.h
FILEStest_yacc+=	calc_code_default.error
FILEStest_yacc+=	calc_code_default.output
FILEStest_yacc+=	calc_code_default.tab.c
FILEStest_yacc+=	calc_code_default.tab.h
FILEStest_yacc+=	calc_code_imports.error
FILEStest_yacc+=	calc_code_imports.output
FILEStest_yacc+=	calc_code_imports.tab.c
FILEStest_yacc+=	calc_code_imports.tab.h
FILEStest_yacc+=	calc_code_provides.error
FILEStest_yacc+=	calc_code_provides.output
FILEStest_yacc+=	calc_code_provides.tab.c
FILEStest_yacc+=	calc_code_provides.tab.h
FILEStest_yacc+=	calc_code_requires.error
FILEStest_yacc+=	calc_code_requires.output
FILEStest_yacc+=	calc_code_requires.tab.c
FILEStest_yacc+=	calc_code_requires.tab.h
FILEStest_yacc+=	calc_code_top.error
FILEStest_yacc+=	calc_code_top.output
FILEStest_yacc+=	calc_code_top.tab.c
FILEStest_yacc+=	calc_code_top.tab.h
FILEStest_yacc+=	calc.error
FILEStest_yacc+=	calc.output
FILEStest_yacc+=	calc.tab.c
FILEStest_yacc+=	calc.tab.h
FILEStest_yacc+=	calc1.error
FILEStest_yacc+=	calc1.output
FILEStest_yacc+=	calc1.tab.c
FILEStest_yacc+=	calc1.tab.h
FILEStest_yacc+=	calc2.error
FILEStest_yacc+=	calc2.output
FILEStest_yacc+=	calc2.tab.c
FILEStest_yacc+=	calc2.tab.h
FILEStest_yacc+=	calc3.error
FILEStest_yacc+=	calc3.output
FILEStest_yacc+=	calc3.tab.c
FILEStest_yacc+=	calc3.tab.h
FILEStest_yacc+=	code_calc.code.c
FILEStest_yacc+=	code_calc.error
FILEStest_yacc+=	code_calc.output
FILEStest_yacc+=	code_calc.tab.c
FILEStest_yacc+=	code_calc.tab.h
FILEStest_yacc+=	code_error.code.c
FILEStest_yacc+=	code_error.error
FILEStest_yacc+=	code_error.output
FILEStest_yacc+=	code_error.tab.c
FILEStest_yacc+=	code_error.tab.h
FILEStest_yacc+=	defines1.calc.c
FILEStest_yacc+=	defines1.calc.h
FILEStest_yacc+=	defines1.error
FILEStest_yacc+=	defines1.output
FILEStest_yacc+=	defines2.calc.c
FILEStest_yacc+=	defines2.calc.h
FILEStest_yacc+=	defines2.error
FILEStest_yacc+=	defines2.output
FILEStest_yacc+=	defines3.calc.c
FILEStest_yacc+=	defines3.calc.h
FILEStest_yacc+=	defines3.error
FILEStest_yacc+=	defines3.output
FILEStest_yacc+=	empty.error
FILEStest_yacc+=	empty.output
FILEStest_yacc+=	empty.tab.c
FILEStest_yacc+=	empty.tab.h
FILEStest_yacc+=	err_syntax1.error
FILEStest_yacc+=	err_syntax1.output
FILEStest_yacc+=	err_syntax1.tab.c
FILEStest_yacc+=	err_syntax1.tab.h
FILEStest_yacc+=	err_syntax10.error
FILEStest_yacc+=	err_syntax10.output
FILEStest_yacc+=	err_syntax10.tab.c
FILEStest_yacc+=	err_syntax10.tab.h
FILEStest_yacc+=	err_syntax11.error
FILEStest_yacc+=	err_syntax11.output
FILEStest_yacc+=	err_syntax11.tab.c
FILEStest_yacc+=	err_syntax11.tab.h
FILEStest_yacc+=	err_syntax12.error
FILEStest_yacc+=	err_syntax12.output
FILEStest_yacc+=	err_syntax12.tab.c
FILEStest_yacc+=	err_syntax12.tab.h
FILEStest_yacc+=	err_syntax13.error
FILEStest_yacc+=	err_syntax13.output
FILEStest_yacc+=	err_syntax13.tab.c
FILEStest_yacc+=	err_syntax13.tab.h
FILEStest_yacc+=	err_syntax14.error
FILEStest_yacc+=	err_syntax14.output
FILEStest_yacc+=	err_syntax14.tab.c
FILEStest_yacc+=	err_syntax14.tab.h
FILEStest_yacc+=	err_syntax15.error
FILEStest_yacc+=	err_syntax15.output
FILEStest_yacc+=	err_syntax15.tab.c
FILEStest_yacc+=	err_syntax15.tab.h
FILEStest_yacc+=	err_syntax16.error
FILEStest_yacc+=	err_syntax16.output
FILEStest_yacc+=	err_syntax16.tab.c
FILEStest_yacc+=	err_syntax16.tab.h
FILEStest_yacc+=	err_syntax17.error
FILEStest_yacc+=	err_syntax17.output
FILEStest_yacc+=	err_syntax17.tab.c
FILEStest_yacc+=	err_syntax17.tab.h
FILEStest_yacc+=	err_syntax18.error
FILEStest_yacc+=	err_syntax18.output
FILEStest_yacc+=	err_syntax18.tab.c
FILEStest_yacc+=	err_syntax18.tab.h
FILEStest_yacc+=	err_syntax19.error
FILEStest_yacc+=	err_syntax19.output
FILEStest_yacc+=	err_syntax19.tab.c
FILEStest_yacc+=	err_syntax19.tab.h
FILEStest_yacc+=	err_syntax2.error
FILEStest_yacc+=	err_syntax2.output
FILEStest_yacc+=	err_syntax2.tab.c
FILEStest_yacc+=	err_syntax2.tab.h
FILEStest_yacc+=	err_syntax20.error
FILEStest_yacc+=	err_syntax20.output
FILEStest_yacc+=	err_syntax20.tab.c
FILEStest_yacc+=	err_syntax20.tab.h
FILEStest_yacc+=	err_syntax21.error
FILEStest_yacc+=	err_syntax21.output
FILEStest_yacc+=	err_syntax21.tab.c
FILEStest_yacc+=	err_syntax21.tab.h
FILEStest_yacc+=	err_syntax22.error
FILEStest_yacc+=	err_syntax22.output
FILEStest_yacc+=	err_syntax22.tab.c
FILEStest_yacc+=	err_syntax22.tab.h
FILEStest_yacc+=	err_syntax23.error
FILEStest_yacc+=	err_syntax23.output
FILEStest_yacc+=	err_syntax23.tab.c
FILEStest_yacc+=	err_syntax23.tab.h
FILEStest_yacc+=	err_syntax24.error
FILEStest_yacc+=	err_syntax24.output
FILEStest_yacc+=	err_syntax24.tab.c
FILEStest_yacc+=	err_syntax24.tab.h
FILEStest_yacc+=	err_syntax25.error
FILEStest_yacc+=	err_syntax25.output
FILEStest_yacc+=	err_syntax25.tab.c
FILEStest_yacc+=	err_syntax25.tab.h
FILEStest_yacc+=	err_syntax26.error
FILEStest_yacc+=	err_syntax26.output
FILEStest_yacc+=	err_syntax26.tab.c
FILEStest_yacc+=	err_syntax26.tab.h
FILEStest_yacc+=	err_syntax27.error
FILEStest_yacc+=	err_syntax27.output
FILEStest_yacc+=	err_syntax27.tab.c
FILEStest_yacc+=	err_syntax27.tab.h
FILEStest_yacc+=	err_syntax3.error
FILEStest_yacc+=	err_syntax3.output
FILEStest_yacc+=	err_syntax3.tab.c
FILEStest_yacc+=	err_syntax3.tab.h
FILEStest_yacc+=	err_syntax4.error
FILEStest_yacc+=	err_syntax4.output
FILEStest_yacc+=	err_syntax4.tab.c
FILEStest_yacc+=	err_syntax4.tab.h
FILEStest_yacc+=	err_syntax5.error
FILEStest_yacc+=	err_syntax5.output
FILEStest_yacc+=	err_syntax5.tab.c
FILEStest_yacc+=	err_syntax5.tab.h
FILEStest_yacc+=	err_syntax6.error
FILEStest_yacc+=	err_syntax6.output
FILEStest_yacc+=	err_syntax6.tab.c
FILEStest_yacc+=	err_syntax6.tab.h
FILEStest_yacc+=	err_syntax7.error
FILEStest_yacc+=	err_syntax7.output
FILEStest_yacc+=	err_syntax7.tab.c
FILEStest_yacc+=	err_syntax7.tab.h
FILEStest_yacc+=	err_syntax7a.error
FILEStest_yacc+=	err_syntax7a.output
FILEStest_yacc+=	err_syntax7a.tab.c
FILEStest_yacc+=	err_syntax7a.tab.h
FILEStest_yacc+=	err_syntax7b.error
FILEStest_yacc+=	err_syntax7b.output
FILEStest_yacc+=	err_syntax7b.tab.c
FILEStest_yacc+=	err_syntax7b.tab.h
FILEStest_yacc+=	err_syntax8.error
FILEStest_yacc+=	err_syntax8.output
FILEStest_yacc+=	err_syntax8.tab.c
FILEStest_yacc+=	err_syntax8.tab.h
FILEStest_yacc+=	err_syntax8a.error
FILEStest_yacc+=	err_syntax8a.output
FILEStest_yacc+=	err_syntax8a.tab.c
FILEStest_yacc+=	err_syntax8a.tab.h
FILEStest_yacc+=	err_syntax9.error
FILEStest_yacc+=	err_syntax9.output
FILEStest_yacc+=	err_syntax9.tab.c
FILEStest_yacc+=	err_syntax9.tab.h
FILEStest_yacc+=	error.error
FILEStest_yacc+=	error.output
FILEStest_yacc+=	error.tab.c
FILEStest_yacc+=	error.tab.h
FILEStest_yacc+=	expr.oxout.error
FILEStest_yacc+=	expr.oxout.output
FILEStest_yacc+=	expr.oxout.tab.c
FILEStest_yacc+=	expr.oxout.tab.h
FILEStest_yacc+=	grammar.dot
FILEStest_yacc+=	grammar.error
FILEStest_yacc+=	grammar.output
FILEStest_yacc+=	grammar.tab.c
FILEStest_yacc+=	grammar.tab.h
FILEStest_yacc+=	help.error
FILEStest_yacc+=	help.output
FILEStest_yacc+=	no_b_opt.error
FILEStest_yacc+=	no_b_opt.output
FILEStest_yacc+=	no_b_opt1.error
FILEStest_yacc+=	no_b_opt1.output
FILEStest_yacc+=	no_code_c.error
FILEStest_yacc+=	no_code_c.output
FILEStest_yacc+=	no_defines.error
FILEStest_yacc+=	no_defines.output
FILEStest_yacc+=	no_graph.error
FILEStest_yacc+=	no_graph.output
FILEStest_yacc+=	no_include.error
FILEStest_yacc+=	no_include.output
FILEStest_yacc+=	no_opts.error
FILEStest_yacc+=	no_opts.output
FILEStest_yacc+=	no_output.error
FILEStest_yacc+=	no_output.output
FILEStest_yacc+=	no_output1.error
FILEStest_yacc+=	no_output1.output
FILEStest_yacc+=	no_output2.error
FILEStest_yacc+=	no_output2.output
FILEStest_yacc+=	no_p_opt.error
FILEStest_yacc+=	no_p_opt.output
FILEStest_yacc+=	no_p_opt1.error
FILEStest_yacc+=	no_p_opt1.output
FILEStest_yacc+=	no_verbose.error
FILEStest_yacc+=	no_verbose.output
FILEStest_yacc+=	nostdin.error
FILEStest_yacc+=	nostdin.output
FILEStest_yacc+=	ok_syntax1.error
FILEStest_yacc+=	ok_syntax1.output
FILEStest_yacc+=	ok_syntax1.tab.c
FILEStest_yacc+=	ok_syntax1.tab.h
FILEStest_yacc+=	pure_calc.error
FILEStest_yacc+=	pure_calc.output
FILEStest_yacc+=	pure_calc.tab.c
FILEStest_yacc+=	pure_calc.tab.h
FILEStest_yacc+=	pure_error.error
FILEStest_yacc+=	pure_error.output
FILEStest_yacc+=	pure_error.tab.c
FILEStest_yacc+=	pure_error.tab.h
FILEStest_yacc+=	quote_calc-s.error
FILEStest_yacc+=	quote_calc-s.output
FILEStest_yacc+=	quote_calc-s.tab.c
FILEStest_yacc+=	quote_calc-s.tab.h
FILEStest_yacc+=	quote_calc.error
FILEStest_yacc+=	quote_calc.output
FILEStest_yacc+=	quote_calc.tab.c
FILEStest_yacc+=	quote_calc.tab.h
FILEStest_yacc+=	quote_calc2-s.error
FILEStest_yacc+=	quote_calc2-s.output
FILEStest_yacc+=	quote_calc2-s.tab.c
FILEStest_yacc+=	quote_calc2-s.tab.h
FILEStest_yacc+=	quote_calc2.error
FILEStest_yacc+=	quote_calc2.output
FILEStest_yacc+=	quote_calc2.tab.c
FILEStest_yacc+=	quote_calc2.tab.h
FILEStest_yacc+=	quote_calc3-s.error
FILEStest_yacc+=	quote_calc3-s.output
FILEStest_yacc+=	quote_calc3-s.tab.c
FILEStest_yacc+=	quote_calc3-s.tab.h
FILEStest_yacc+=	quote_calc3.error
FILEStest_yacc+=	quote_calc3.output
FILEStest_yacc+=	quote_calc3.tab.c
FILEStest_yacc+=	quote_calc3.tab.h
FILEStest_yacc+=	quote_calc4-s.error
FILEStest_yacc+=	quote_calc4-s.output
FILEStest_yacc+=	quote_calc4-s.tab.c
FILEStest_yacc+=	quote_calc4-s.tab.h
FILEStest_yacc+=	quote_calc4.error
FILEStest_yacc+=	quote_calc4.output
FILEStest_yacc+=	quote_calc4.tab.c
FILEStest_yacc+=	quote_calc4.tab.h
FILEStest_yacc+=	rename_debug.c
FILEStest_yacc+=	rename_debug.error
FILEStest_yacc+=	rename_debug.h
FILEStest_yacc+=	rename_debug.i
FILEStest_yacc+=	rename_debug.output
FILEStest_yacc+=	stdin1.calc.c
FILEStest_yacc+=	stdin1.error
FILEStest_yacc+=	stdin1.output
FILEStest_yacc+=	stdin2.calc.c
FILEStest_yacc+=	stdin2.error
FILEStest_yacc+=	stdin2.output
FILEStest_yacc+=	varsyntax_calc1.error
FILEStest_yacc+=	varsyntax_calc1.output
FILEStest_yacc+=	varsyntax_calc1.tab.c
FILEStest_yacc+=	varsyntax_calc1.tab.h

.include <bsd.test.mk>
