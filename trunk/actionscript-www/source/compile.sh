#!/bin/sh

#NAME="test"
FILE=$1
NAME=${FILE%.*}

#DEBUG="-d"
DEBUG=""

# edit this path according to your setup
TAMARIN_DIR="/home/victor/TamarinServer/workspace/tamarin-fcgi/tamarin-central"


java -jar $TAMARIN_DIR/utils/asc.jar \
	-import $TAMARIN_DIR/core/builtin.abc \
	-import $TAMARIN_DIR/fcgishell/shell_toplevel.abc $DEBUG $NAME.as

rm -f ../$NAME.abc
mv $NAME.abc ../$NAME.abc
