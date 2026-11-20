#!/bin/sh
# builds all nodes from default_envs and uploads them over the network, see README
#   ./ota_all.sh              all nodes
#   ./ota_all.sh og_ost ug    only these
set -e
cd "$(dirname "$0")"
PIO="${PIO:-$HOME/.platformio/penv/bin/pio}"
if [ $# -eq 0 ]; then
    exec "$PIO" run -t ota
fi
args=""
for node in "$@"; do
    args="$args -e $node"
done
exec "$PIO" run $args -t ota
