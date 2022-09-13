/*
 * Source code : RingBuf.c
 * Module      : Common/DS
 * Purpose     : Implement Ring Buffer FIFO queue.
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

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "RingBuf.h"


// ----------------------------------------------------------------------
// Private (Static) functions declaration
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Public functions implementation
// ----------------------------------------------------------------------


HResult RingBuf_Create(RingBuf* ring_buf, const size_t _capacity, const size_t _elemsize)
{
    HResult retcode = HResult_OK;

    if (ring_buf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    if (_capacity == 0 || _elemsize == 0)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    memset(ring_buf, 0, sizeof(RingBuf));
    ring_buf->capacity = _capacity;
    ring_buf->elemsize = _elemsize;

    ring_buf->buffer = (byte*)malloc(_capacity * _elemsize);
    if (!ring_buf->buffer)
    {
        retcode = HResult_ALLOC_FAIL;
        goto EXIT;
    }
    memset((void*)ring_buf->buffer, 0, _capacity * _elemsize);

EXIT:
    return retcode;
}


void RingBuf_Destroy(RingBuf* ring_buf)
{
    if (ring_buf)
    {
        if (ring_buf->buffer)
        {
            free(ring_buf->buffer);
            ring_buf->buffer = NULL;
        }

        memset(ring_buf, 0, sizeof(RingBuf));
    }
}


HResult RingBuf_Push(RingBuf* ring_buf, void* const _elem, const size_t _elem_count)
{
    HResult retcode = HResult_OK;
    size_t write_capacity;
    size_t cp_count;
    byte* dst;
    byte* src;

    if (_elem_count == 0)
    {
        goto EXIT;
    }

    if (ring_buf == NULL || _elem == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }
    

    if (_elem_count > ring_buf->capacity)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    write_capacity = RingBuf_GetWriteCapacity(ring_buf);
    if (_elem_count > write_capacity)
    {
        retcode = HResult_DATA_INVALID;
        goto EXIT;
    }

    if ((ring_buf->cycle == 1) || ((ring_buf->cycle == 0) && (_elem_count <= ring_buf->capacity - ring_buf->wpos)))
    {
        dst = ring_buf->buffer + (ring_buf->wpos * ring_buf->elemsize);
        memcpy((void*)dst, (void*)_elem, _elem_count * ring_buf->elemsize);

        if (ring_buf->cycle == 0)
        {
            if (_elem_count < ring_buf->capacity - ring_buf->wpos)
            {
                ring_buf->wpos += _elem_count;
            }
            else // Condition: _elem_count == ring_buf->capacity - ring_buf->wpos
            {
                ring_buf->cycle = 1;
                ring_buf->wpos = 0;
            }
        }
        else // Condition: ring_buf->cycle == 1
        {
            ring_buf->wpos += _elem_count;
        }
    }
    else // Condition: (ring_buf->cycle == 0) && (_elem_count > ring_buf->capacity - ring_buf->wpos)
    {
        dst = ring_buf->buffer + (ring_buf->wpos * ring_buf->elemsize);
        cp_count = ring_buf->capacity - ring_buf->wpos;
        memcpy((void*)dst, (void*)_elem, cp_count * ring_buf->elemsize);

        src = (byte*)_elem + (cp_count * ring_buf->elemsize);
        cp_count = _elem_count - cp_count;
        memcpy((void*)ring_buf->buffer, (void*)src, cp_count * ring_buf->elemsize);

        ring_buf->cycle = 1;
        ring_buf->wpos = cp_count;
    }

EXIT:
    return retcode;
}


HResult RingBuf_Peek(RingBuf* ring_buf, byte* _elem_buf, const size_t _elem_count)
{
    HResult retcode = HResult_OK;
    size_t read_capacity;
    size_t cp_count;
    byte* dst;
    byte* src;

    if (_elem_count == 0)
    {
        goto EXIT;
    }

    if (ring_buf == NULL || _elem_buf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    read_capacity = RingBuf_GetReadCapacity(ring_buf);
    if (_elem_count > read_capacity)
    {
        retcode = HResult_DATA_INVALID;
        goto EXIT;
    }

    if ((ring_buf->cycle == 0) || ((ring_buf->cycle == 1) && (_elem_count <= ring_buf->capacity - ring_buf->rpos)))
    {
        src = ring_buf->buffer + (ring_buf->rpos * ring_buf->elemsize);
        memcpy((void*)_elem_buf, (void*)src, _elem_count * ring_buf->elemsize);
    }
    else // Condition: (ring_buf->cycle == 1) && (_elem_count > ring_buf->capacity - ring_buf->rpos)
    {
        src = ring_buf->buffer + (ring_buf->rpos * ring_buf->elemsize);
        cp_count = ring_buf->capacity - ring_buf->rpos;
        memcpy((void*)_elem_buf, (void*)src, cp_count * ring_buf->elemsize);

        dst = _elem_buf + (cp_count * ring_buf->elemsize);
        cp_count = _elem_count - cp_count;
        memcpy((void*)dst, (void*)ring_buf->buffer, cp_count * ring_buf->elemsize);
    }

EXIT:
    return retcode;
}


HResult RingBuf_Pop(RingBuf* ring_buf, byte* _elem_buf, const size_t _elem_count)
{
    HResult retcode = HResult_OK;
    size_t read_capacity;
    size_t cp_count;
    byte* dst;
    byte* src;

    if (_elem_count == 0)
    {
        goto EXIT;
    }

    if (ring_buf == NULL || _elem_buf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    read_capacity = RingBuf_GetReadCapacity(ring_buf);
    if (_elem_count > read_capacity)
    {
        retcode = HResult_DATA_INVALID;
        goto EXIT;
    }

    if ((ring_buf->cycle == 0) || ((ring_buf->cycle == 1) && (_elem_count <= ring_buf->capacity - ring_buf->rpos)))
    {
        src = ring_buf->buffer + (ring_buf->rpos * ring_buf->elemsize);
        memcpy((void*)_elem_buf, (void*)src, _elem_count * ring_buf->elemsize);

        if (ring_buf->cycle == 1)
        {
            if (_elem_count < ring_buf->capacity - ring_buf->rpos)
            {
                ring_buf->rpos += _elem_count;
            }
            else // Condition: _elem_count == ring_buf->capacity - ring_buf->rpos
            {
                ring_buf->cycle = 0;
                ring_buf->rpos = 0;
            }
        }
        else // Condition: ring_buf->cycle == 0
        {
            ring_buf->rpos += _elem_count;
        }
    }
    else // Condition: (ring_buf->cycle == 1) && (_elem_count > ring_buf->capacity - ring_buf->rpos)
    {
        src = ring_buf->buffer + (ring_buf->rpos * ring_buf->elemsize);
        cp_count = ring_buf->capacity - ring_buf->rpos;
        memcpy((void*)_elem_buf, (void*)src, cp_count * ring_buf->elemsize);

        dst = _elem_buf + (cp_count * ring_buf->elemsize);
        cp_count = _elem_count - cp_count;
        memcpy((void*)dst, (void*)ring_buf->buffer, cp_count * ring_buf->elemsize);

        ring_buf->cycle = 0;
        ring_buf->rpos = cp_count;
    }

EXIT:
    return retcode;
}


size_t RingBuf_GetWriteCapacity(RingBuf* ring_buf)
{
    size_t write_capacity = 0;

    assert(ring_buf != NULL);

    if (ring_buf->cycle == 0)
    {
        if (ring_buf->rpos > ring_buf->wpos)
        {
            // retcode: HResult_DATA_INVALID
            goto EXIT;
        }

        write_capacity = ring_buf->capacity - ring_buf->wpos + ring_buf->rpos;
    }
    else // Condition: ring_buf->cycle == 1
    {
        if (ring_buf->rpos < ring_buf->wpos)
        {
            // retcode: HResult_DATA_INVALID
            goto EXIT;
        }

        write_capacity = ring_buf->rpos - ring_buf->wpos;
    }

EXIT:
    return write_capacity;
}


size_t RingBuf_GetReadCapacity(RingBuf* ring_buf)
{
    size_t read_capacity = 0;

    assert(ring_buf != NULL);
    if (!ring_buf)
    {
        goto EXIT;
    }

    if (ring_buf->cycle == 0)
    {
        if (ring_buf->rpos > ring_buf->wpos)
        {
            // retcode: HResult_DATA_INVALID
            goto EXIT;
        }

        read_capacity = ring_buf->wpos - ring_buf->rpos;
    }
    else // Condition: ring_buf->cycle == 1
    {
        if (ring_buf->rpos < ring_buf->wpos)
        {
            // retcode: HResult_DATA_INVALID
            goto EXIT;
        }

        read_capacity = ring_buf->capacity - ring_buf->rpos + ring_buf->wpos;
    }

EXIT:
    return read_capacity;
}


// ============================================
// Methods for stream message receiving [Begin]
// ============================================

// Return value: ssize_t | (1) >= 0: found; (2) -1: not found; (3) < -1: error occurs;
ssize_t RingBuf_SeekElem(RingBuf* ring_buf, void* const _tofind_elem)
{
    ssize_t seek_steps = 0;
    size_t read_capacity = 0;
    size_t idx_step = 0;
    int match_result = -1;
    byte* src = NULL;

    if (ring_buf == NULL || _tofind_elem == NULL)
    {
        // detailed error information: HResult_PARAM_NULL
        seek_steps = -2;
        goto EXIT;
    }

    read_capacity = RingBuf_GetReadCapacity(ring_buf);
    if (read_capacity == 0)
    {
        // detailed error information: ring_buf has no element to read.
        seek_steps = -3;
        goto EXIT;
    }

    if (ring_buf->cycle == 0)
    {
        for (idx_step = ring_buf->rpos; idx_step < ring_buf->wpos; idx_step++)
        {
            src = ring_buf->buffer + (idx_step * ring_buf->elemsize);
            // Reference: [1] https://www.man7.org/linux/man-pages/man3/memcmp.3.html
            //            [2] https://www.tutorialspoint.com/c_standard_library/c_function_memcmp.htm
            match_result = memcmp(src, _tofind_elem, ring_buf->elemsize);
            if (match_result == 0)
            {
                seek_steps = (ssize_t)idx_step - (ssize_t)ring_buf->rpos;
                goto EXIT;
            }
        }

        // Did not find
        seek_steps = -1;
        goto EXIT;
    }
    else // Condition: (ring_buf->cycle == 1)
    {
        for (idx_step = ring_buf->rpos; idx_step < ring_buf->capacity; idx_step++)
        {
            src = ring_buf->buffer + (idx_step * ring_buf->elemsize);
            match_result = memcmp(src, _tofind_elem, ring_buf->elemsize);
            if (match_result == 0)
            {
                seek_steps = (ssize_t)ring_buf->capacity - (ssize_t)ring_buf->rpos - 1;
                goto EXIT;
            }
        }

        seek_steps = (ssize_t)ring_buf->capacity - (ssize_t)ring_buf->rpos;
        for (idx_step = 0; idx_step < ring_buf->wpos; idx_step++)
        {
            src = ring_buf->buffer + (idx_step * ring_buf->elemsize);
            match_result = memcmp(src, _tofind_elem, ring_buf->elemsize);
            if (match_result == 0)
            {
                seek_steps += ((ssize_t)idx_step + 1);
                goto EXIT;
            }
        }

        // Did not find
        seek_steps = -1;
        goto EXIT;
    }

EXIT:
    return seek_steps;
}


HResult RingBuf_RejectElements(RingBuf* ring_buf, const size_t reject_count)
{
    HResult retcode = HResult_OK;
    size_t read_capacity = 0;

    if (reject_count == 0)
    {
        goto EXIT;
    }

    if (ring_buf == NULL)
    {
        retcode = HResult_PARAM_NULL;
        goto EXIT;
    }

    read_capacity = RingBuf_GetReadCapacity(ring_buf);
    if (reject_count > read_capacity)
    {
        retcode = HResult_PARAM_OUTRANGE;
        goto EXIT;
    }

    if (ring_buf->cycle == 0)
    {
        ring_buf->rpos += reject_count;
    }
    else // Condition: (ring_buf->cycle == 1)
    {
        if (ring_buf->rpos < ring_buf->capacity - reject_count)
        {
            ring_buf->rpos += reject_count;
        }
        else
        {
            ring_buf->cycle = 0;
            ring_buf->rpos = reject_count - (ring_buf->capacity - ring_buf->rpos);
        }
    }

EXIT:
    return retcode;
}

// ============================================
// Methods for stream message receiving [END  ]
// ============================================


// ----------------------------------------------------------------------
// Private (Static) functions implementation
// ----------------------------------------------------------------------
