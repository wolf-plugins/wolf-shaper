NAMESPACE="$1"
FONT_NAME="$2"
FILE_NAME="$3"
BASE64="$(cat ${FILE_NAME} | base64 --wrap 0)"
SIZE=${#BASE64}

cat << END > "${FONT_NAME}.cpp"
/* (Auto-generated binary data file). */

#include "${FONT_NAME}.hpp"

static const unsigned char temp1[] = {$(echo ${BASE64} | ./getchunk)};
const char* ${NAMESPACE}::${FONT_NAME} = (const char*) temp1;
END

cat << END > "${FONT_NAME}.hpp"
/* (Auto-generated binary data file). */

#ifndef BINARY_${NAMESPACE}_FONT_${FONT_NAME^^}_INCLUDED
#define BINARY_${NAMESPACE}_FONT_${FONT_NAME^^}_INCLUDED

namespace ${NAMESPACE}
{
	extern const char* ${FONT_NAME};
	const unsigned int ${FONT_NAME}_size = ${SIZE};
}

#endif
END
