#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

static const char *JSON_STRING =
	"{\"payload\": {\"cmd\": \"load\", \"type\": \"int\", \"data\": 10}, \"checksum\": \"test_checksum\"}";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main() {
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "payload") == 0) {
			/* payload is an object */
			if (t[i + 1].type != JSMN_OBJECT) {
				printf("Object expected\n");
				return 1;
			}
			i++;
			int p_size = t[i].size;
			int base = i;
			i++;
			for (; i < p_size*2 + base; i++) {
				if (jsoneq(JSON_STRING, &t[i], "cmd") == 0) {
					/* cmd is a string */
					printf("- cmd: %.*s\n", t[i + 1].end - t[i + 1].start,
						JSON_STRING + t[i + 1].start);
					i++;
				} else if (jsoneq(JSON_STRING, &t[i], "type") == 0) {
					/* type is a string */
					printf("- type: %.*s\n", t[i + 1].end - t[i + 1].start,
						JSON_STRING + t[i + 1].start);
					i++;
				}
				else if (jsoneq(JSON_STRING, &t[i], "data") == 0) {
					/* data is a number */
					printf("- data: %.*s\n", t[i + 1].end - t[i + 1].start,
						JSON_STRING + t[i + 1].start);
				}
				else {
					printf("Unexpected key in payload: %.*s\n", t[i].end-t[i].start,
					JSON_STRING + t[i].start);
				}
			}
		} else if (jsoneq(JSON_STRING, &t[i], "checksum") == 0) {
			/* checksum is in hex */
			printf("- checksum: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else {
			printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
					JSON_STRING + t[i].start);
		}
	}
	return EXIT_SUCCESS;
}
