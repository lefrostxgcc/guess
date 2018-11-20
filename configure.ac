#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([guess], [1.0], [chipz68q@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects])
AC_CONFIG_SRCDIR([src/server/main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_AR
LT_INIT

# Checks for header files.
AC_CHECK_HEADERS([netinet/in.h stdlib.h sys/socket.h unistd.h])

# Checks for library functions.
AC_CHECK_FUNCS([socket])

AC_CONFIG_FILES([Makefile
					lib/Makefile
					src/Makefile
					src/server/Makefile
					src/client/Makefile])
AC_OUTPUT
