/* 
 * Copyright (c) 2016 Lammert Bies
 * Copyright (c) 2013-2016 the Civetweb developers
 * Copyright (c) 2004-2013 Sergey Lyubka
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */



#include "libhttp-private.h"



/*
 * int XX_httplib_consume_socket( struct mg_context *ctx, struct socket *sp, int thread_index );
 *
 * The function XX_httplib_consume_socket() takes an accepted socket from the
 * queue for further processing.
 */

#if defined(ALTERNATIVE_QUEUE)

int XX_httplib_consume_socket( struct mg_context *ctx, struct socket *sp, int thread_index ) {

	ctx->client_socks[thread_index].in_use = 0;
	event_wait(ctx->client_wait_events[thread_index]);
	*sp = ctx->client_socks[thread_index];

	return !ctx->stop_flag;

}  /* XX_httplib_consume_socket */

#else /* ALTERNATIVE_QUEUE */

/* Worker threads take accepted socket from the queue */
int XX_httplib_consume_socket( struct mg_context *ctx, struct socket *sp, int thread_index ) {

#define QUEUE_SIZE(ctx) ((int)(ARRAY_SIZE(ctx->queue)))

	(void)thread_index;

	pthread_mutex_lock(&ctx->thread_mutex);

	/* If the queue is empty, wait. We're idle at this point. */
	while (ctx->sq_head == ctx->sq_tail && ctx->stop_flag == 0) {
		pthread_cond_wait(&ctx->sq_full, &ctx->thread_mutex);
	}

	/* If we're stopping, sq_head may be equal to sq_tail. */
	if (ctx->sq_head > ctx->sq_tail) {
		/* Copy socket from the queue and increment tail */
		*sp = ctx->queue[ctx->sq_tail % QUEUE_SIZE(ctx)];
		ctx->sq_tail++;

		/* Wrap pointers if needed */
		while (ctx->sq_tail > QUEUE_SIZE(ctx)) {
			ctx->sq_tail -= QUEUE_SIZE(ctx);
			ctx->sq_head -= QUEUE_SIZE(ctx);
		}
	}

	pthread_cond_signal(&ctx->sq_empty);
	pthread_mutex_unlock(&ctx->thread_mutex);

	return !ctx->stop_flag;
#undef QUEUE_SIZE

}  /* XX_httplib_consume_socket */

#endif /* ALTERNATIVE_QUEUE */