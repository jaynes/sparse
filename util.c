/* Copyright 2013 Brian Swetland <swetland@frotz.net>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "util.h"

int readx(int fd, void *_data, size_t len) {
	ssize_t r;
	u8 *data = _data;
	while (len > 0) {
		r = read(fd, data, len);
		if (r < 0) {
			if (errno == EINTR)
				continue;
			return -1;
		}
		len -= r;
		data += r;
	}
	return 0;
}

int writex(int fd, void *_data, size_t len) {
	ssize_t r;
	u8 *data = _data;
	while (len > 0) {
		r = write(fd, data, len);
		if (r < 0) {
			if (errno == EINTR)
				continue;
			return -1;
		}
		len -= r;
		data += r;
	}
	return 0;
}

int copyx(int fin, int fout, size_t len, u8 *buf, size_t max) {
	while (len > 0) {
		size_t xfer = max > len ? len : max;
		if (readx(fin, buf, xfer))
			return -1;
		if (writex(fout, buf, xfer))
			return -1;
		len -= xfer;
	}
	return 0;
}
