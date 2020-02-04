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

#include "ringbuffer.h"

/* Must be called with interrupt disabled, or from interrupt context */
void ring_buf_in(struct ring_buf_s *buffer, uint8_t val)
{
    buffer->buf[buffer->buf_in_i++] = val;
    buffer->buf_in_i &= BUFFER_MASK;
    buffer->buf_count++;
    if (buffer->buf_count == RINGBUFFER_SIZE)
    {
        buffer->buf_full = 1;
    }
}

/* Must be called with interrupt disabled, or from interrupt context */
uint8_t ring_buf_out(struct ring_buf_s *buffer)
{
    uint8_t byte;
    byte = buffer->buf[buffer->buf_out_i++];
    buffer->buf_out_i &= BUFFER_MASK;
    buffer->buf_count--;
    buffer->buf_full = 0;
    return byte;
}

uint8_t ring_buf_count(struct ring_buf_s *buffer)
{
    return buffer->buf_count;
}

uint8_t ring_buf_full(struct ring_buf_s *buffer)
{
    return buffer->buf_full;
}
