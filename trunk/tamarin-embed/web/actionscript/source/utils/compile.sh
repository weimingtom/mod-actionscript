#!/bin/sh

FILE=$1
NAME=${FILE%.*}

DEBUG="-d"


#TAMARIN_DIR="/home/victor/TamarinServer/workspace/tamarin-embed/tamarin-central"
#java -jar $TAMARIN_DIR/utils/asc.jar \
#	-import $TAMARIN_DIR/core/builtin.abc \
#	-import $TAMARIN_DIR/shell/shell_toplevel.abc \
#	$DEBUG -strict -md $NAME.as

java -jar utils/asc.jar \
	-import utils/builtin.abc \
	-import utils/shell_toplevel.abc \
	$DEBUG -strict -md $NAME.as

rm -f ../$NAME.abc
mv $NAME.abc ../$NAME.abc
