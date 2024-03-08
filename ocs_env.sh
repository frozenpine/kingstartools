#!/bin/bash

cd "`dirname $0`"
BASE_DIR="`pwd`"
OCS_BASE="${BASE_DIR}/ocs15_0"

LIB_BASE="${OCS_BASE}/lib"
LIB3P_BASE="${OCS_BASE}/lib3p64"
PY_MODULE_BASE="${OCS_BASE}/python/python31_64r/lib"

VENV_BASE="${BASE_DIR}/.venv"

export LD_LIBRARY_PATH="${LIB_BASE}":"${LIB3P_BASE}":"${PY_MODULE_BASE}":${LD_LIBRARY_PATH}

export SYBASE="${OCS_BASE}"
# source "${VENV_BASE}/bin/activate"
