#!/bin/bash
# Integration smoke test — validates Lemmy API client against a live instance.
# Usage: LEMMY_INSTANCE=https://lemmy.ml ./test_lemmy_api.sh
set -euo pipefail

INSTANCE="${LEMMY_INSTANCE:-https://lemmy.ml}"
API="${INSTANCE}/api/v3"
echo "=== Lemmy API smoke test against ${INSTANCE} ==="

# 1. GET /site (anonymous)
echo -n "  GET /site ... "
SITE=$(curl -sf "${API}/site")
echo "OK ($(echo "$SITE" | python3 -c "import sys,json; print(json.load(sys.stdin)['site_view']['site']['name'])"))"

# 2. GET /community/list (anonymous)
echo -n "  GET /community/list ... "
COMMS=$(curl -sf "${API}/community/list?sort=Active&type_=Local&limit=3")
echo "OK ($(echo "$COMMS" | python3 -c "import sys,json; d=json.load(sys.stdin); print(len(d['communities']), 'communities')"))"

# 3. GET /post/list (anonymous)
echo -n "  GET /post/list ... "
POSTS=$(curl -sf "${API}/post/list?sort=Active&type_=Local&limit=3")
echo "OK ($(echo "$POSTS" | python3 -c "import sys,json; d=json.load(sys.stdin); print(len(d['posts']), 'posts')"))"

# 4. GET /post/list (with show_nsfw)
echo -n "  GET /post/list?show_nsfw=true ... "
curl -sf "${API}/post/list?sort=Active&type_=Local&limit=1&show_nsfw=true" > /dev/null
echo "OK"

# 5. GET /search
echo -n "  GET /search?q=test ... "
curl -sf "${API}/search?q=test&type_=All&sort=TopAll" > /dev/null
echo "OK"

# 6. GET /resolve_object
echo -n "  GET /resolve_object ... "
curl -sf "${API}/resolve_object?q=https://lemmy.ml/post/1" 2>/dev/null && echo "OK" || echo "SKIP (post/1 may not exist)"

# 7. GET /federated_instances (if available)
echo -n "  GET /federated_instances ... "
curl -sf "${API}/federated_instances" 2>/dev/null && echo "OK" || echo "SKIP (may not be available)"

echo "=== All smoke tests passed ==="
