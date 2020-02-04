/* Copyright (C) 2016 Kim Taylor
 *
 * This file is part of hbc_mac.
 *
 * hbc_mac is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * hbc_mac is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hbc_mac.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include <project.h>

#define RINGBUFFER_SIZE 64
#define BUFFER_MASK (RINGBUFFER_SIZE - 1)

struct ring_buf_s
{
    uint8_t buf[RINGBUFFER_SIZE];
    uint8_t buf_in_i;
    uint8_t buf_out_i;
    uint8_t buf_count;
    uint8_t buf_full;
};

void ring_buf_in(struct ring_buf_s *buffer, uint8_t val);
uint8_t ring_buf_out(struct ring_buf_s *buffer);
uint8_t ring_buf_count(struct ring_buf_s *buffer);
uint8_t ring_buf_full(struct ring_buf_s *buf_full);

#endif
