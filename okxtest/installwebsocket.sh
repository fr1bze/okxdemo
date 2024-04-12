WEBSOCKETPP_VERSION=$1
WEBSOCKETPP_SRC_DIR=${HOME}/src/websocketpp
WEBSOCKETPP_TAR_GZ_FILE=${WEBSOCKETPP_VERSION}.tar.gz

echo "Download WebSocket++ ${WEBSOCKETPP_VERSION} to ${WEBSOCKETPP_SRC_DIR}"

mkdir -p ${WEBSOCKETPP_SRC_DIR}
wget https://github.com/zaphoyd/websocketpp/archive/${WEBSOCKETPP_TAR_GZ_FILE} --directory-prefix=${WEBSOCKETPP_SRC_DIR}
tar -xz -C ${WEBSOCKETPP_SRC_DIR} -f ${WEBSOCKETPP_SRC_DIR}/${WEBSOCKETPP_TAR_GZ_FILE}
rm ${WEBSOCKETPP_SRC_DIR}/${WEBSOCKETPP_TAR_GZ_FILE}