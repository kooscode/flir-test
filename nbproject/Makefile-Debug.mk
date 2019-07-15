#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/e1dda48/camera_async.o \
	${OBJECTDIR}/_ext/e1dda48/camera_base.o \
	${OBJECTDIR}/_ext/e1dda48/camera_flir_blackfly.o \
	${OBJECTDIR}/_ext/e1dda48/camera_flir_blackfly_system.o \
	${OBJECTDIR}/_ext/e1dda48/camera_usb.o \
	${OBJECTDIR}/_ext/e1dda48/error_base.o \
	${OBJECTDIR}/_ext/e1dda48/filetools.o \
	${OBJECTDIR}/_ext/e1dda48/stopwatch.o \
	${OBJECTDIR}/_ext/e1dda48/thread_base.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-DTC_USE_BLACKFLY
CXXFLAGS=-DTC_USE_BLACKFLY

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lSpinnaker `pkg-config --libs opencv`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/flir-test

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/flir-test: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/flir-test ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/e1dda48/camera_async.o: ../libterraclear/src/camera_async.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I/usr/include/spinnaker `pkg-config --cflags opencv`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/camera_async.o ../libterraclear/src/camera_async.cpp

${OBJECTDIR}/_ext/e1dda48/camera_base.o: ../libterraclear/src/camera_base.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I/usr/include/spinnaker `pkg-config --cflags opencv`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/camera_base.o ../libterraclear/src/camera_base.cpp

${OBJECTDIR}/_ext/e1dda48/camera_flir_blackfly.o: ../libterraclear/src/camera_flir_blackfly.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I/usr/include/spinnaker `pkg-config --cflags opencv`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/camera_flir_blackfly.o ../libterraclear/src/camera_flir_blackfly.cpp

${OBJECTDIR}/_ext/e1dda48/camera_flir_blackfly_system.o: ../libterraclear/src/camera_flir_blackfly_system.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I/usr/include/spinnaker `pkg-config --cflags opencv`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/camera_flir_blackfly_system.o ../libterraclear/src/camera_flir_blackfly_system.cpp

${OBJECTDIR}/_ext/e1dda48/camera_usb.o: ../libterraclear/src/camera_usb.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I/usr/include/spinnaker `pkg-config --cflags opencv`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/camera_usb.o ../libterraclear/src/camera_usb.cpp

${OBJECTDIR}/_ext/e1dda48/error_base.o: ../libterraclear/src/error_base.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I/usr/include/spinnaker `pkg-config --cflags opencv`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/error_base.o ../libterraclear/src/error_base.cpp

${OBJECTDIR}/_ext/e1dda48/filetools.o: ../libterraclear/src/filetools.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I/usr/include/spinnaker `pkg-config --cflags opencv`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/filetools.o ../libterraclear/src/filetools.cpp

${OBJECTDIR}/_ext/e1dda48/stopwatch.o: ../libterraclear/src/stopwatch.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I/usr/include/spinnaker `pkg-config --cflags opencv`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/stopwatch.o ../libterraclear/src/stopwatch.cpp

${OBJECTDIR}/_ext/e1dda48/thread_base.o: ../libterraclear/src/thread_base.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I/usr/include/spinnaker `pkg-config --cflags opencv`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/thread_base.o ../libterraclear/src/thread_base.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I.. -I/usr/include/spinnaker `pkg-config --cflags opencv`   -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
