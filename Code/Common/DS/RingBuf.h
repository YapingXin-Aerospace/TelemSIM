/*
 * Source code : RingBuf.h
 * Module      : Common/DS
 * Purpose     : Provide interface for Ring Buffer FIFO queue.
 * Author      : Yaping Xin <Yaping.Xin@foxmail.com>
 * Create Date : 2020-05-28
 * Description : NULL
 * History     : NULL
 *
 * Copyright (C) 2020 5-Dim StarLux Co., Ltd
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef _INC_COMMON_DS_RingBuf_H
#define _INC_COMMON_DS_RingBuf_H

#include "../datatypes.h"
#include "../PosixInterface.h"


typedef struct
{
    size_t capacity;
    size_t elemsize;
    size_t wpos;
    size_t rpos;
    uint8_t cycle;
    byte* buffer;

} RingBuf;


HResult RingBuf_Create(RingBuf* ring_buf, const size_t _capacity, const size_t _elemsize);
void RingBuf_Destroy(RingBuf* ring_buf);

HResult RingBuf_Push(RingBuf* ring_buf, void* const _elem, const size_t _elem_count);
HResult RingBuf_Peek(RingBuf* ring_buf, byte* _elem_buf, const size_t _elem_count);
HResult RingBuf_Pop(RingBuf* ring_buf, byte* _elem_buf, const size_t _elem_count);
size_t RingBuf_GetWriteCapacity(RingBuf* ring_buf);
size_t RingBuf_GetReadCapacity(RingBuf* ring_buf);

// ============================================
// Methods for stream message receiving [Begin]
// ============================================

// Return value: ssize_t | (1) >= 0: found; (2) -1: not found; (3) < -1: error occurs;
ssize_t RingBuf_SeekElem(RingBuf* ring_buf, void* const _tofind_elem);

HResult RingBuf_RejectElements(RingBuf* ring_buf, const size_t reject_count);

// ============================================
// Methods for stream message receiving [END  ]
// ============================================

#endif // !_INC_COMMON_DS_RingBuf_H
