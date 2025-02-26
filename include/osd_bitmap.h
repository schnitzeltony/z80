/***************************************************************************************
 *
 * osd_bitmap.h	Operating System surface handling
 *
 * Copyright by Juergen Buchmueller <pullmoll@t-online.de>
 * Copyright 2015 by Andreas Müller <schnitzeltony@googlemail.com>
 *
 ***************************************************************************************/
#if !defined(_OSD_BITMAP_H_INCLUDED_)
#define	_OSD_BITMAP_H_INCLUDED_

#include "system.h"
#include "osd.h"

/** @brief Defines the osd_bitmap_t type */
typedef struct osd_bitmap_s {
	/** @brief number of dirty rectangles */
	uint32_t dirty_count;
	/** @brief maximum number of dirty rectangles */
	uint32_t dirty_alloc;
	/** @brief dirty rectangles */
	SDL_Rect *dirty;
	/** @brief bitmap surface */
	SDL_Surface *surface;
	/** @brief bitmap texture volatile see doc for SDL_LockTexture */
	SDL_Texture *texture;
}	osd_bitmap_t;

extern osd_bitmap_t *frame;

extern int32_t osd_bitmap_alloc(osd_bitmap_t **pbitmap,	SDL_Window *window,
	int32_t width, int32_t height);
	
extern void osd_bitmap_free(osd_bitmap_t **pbitmap);

extern int32_t osd_bitmap_create_texture(osd_bitmap_t *bitmap, SDL_Renderer *renderer);

static __inline void osd_bitmap_dirty(osd_bitmap_t *bitmap, SDL_Rect *dst);

extern void osd_bitmap_hline(osd_bitmap_t *bitmap, 
	int32_t x, int32_t y, int32_t l, int32_t w, uint32_t pixel);

extern void osd_bitmap_vline(osd_bitmap_t *bitmap, 
	int32_t x, int32_t y, int32_t l, int32_t w, uint32_t pixel);

extern void osd_bitmap_framerect(osd_bitmap_t *bitmap, 
	int32_t x, int32_t y, int32_t w, int32_t h,
	int32_t corners, uint32_t lw, uint32_t tl, uint32_t br);
	
extern void osd_bitmap_fillrect(osd_bitmap_t *bitmap, 
	int32_t x, int32_t y, int32_t w, int32_t h, uint32_t pixel);

extern uint32_t oss_bitmap_get_pix_val(osd_bitmap_t *bitmap, 
	uint8_t r, uint8_t g, uint8_t b, uint8_t a);

extern void osd_bitmap_render(osd_bitmap_t *bitmap, SDL_Renderer* renderer);

/**
 * @brief add a dirty rectangle to a bitmap's surface
 *
 * @param bitmap bitmap handle to add the dirty rectangle to
 * @param dst destination rectangle
 */
static __inline void osd_bitmap_dirty(osd_bitmap_t *bitmap, SDL_Rect *dst)
{
	/* have not found a way to use (enhace performance) with sdl2 texture */
	return;
	
	SDL_Rect *r;
	uint32_t n;

	/* check if area is already dirty */
	for (n = 0; n < bitmap->dirty_count; n++) {
		r = &bitmap->dirty[n];
		if (dst->y >= r->y &&
			dst->y + dst->h <= r->y + r->h &&
			dst->x >= r->x &&
			dst->x + dst->w <= r->x + r->w) {
				return;
			}
	}
	/* try to combine the dirty rectangle with an existing one */
	for (n = 0; n < bitmap->dirty_count; n++) {
		r = &bitmap->dirty[n];
		if (dst->y == r->y && dst->h == r->h) {
			if (r->x + r->w == dst->x) {
				r->w = r->w + dst->w;
				return;
			}
			if (dst->x + dst->w == r->x) {
				r->x = dst->x;
				r->w = r->w + dst->w;
				return;
			}
		}
		if (dst->x == r->x && dst->w == r->w) {
			if (r->y + r->h == dst->y) {
				r->h = r->h + dst->h;
				return;
			}
			if (dst->y + dst->h == r->y) {
				r->y = dst->y;
				r->h = r->h + dst->h;
				return;
			}
		}
	}
	if (bitmap->dirty_count >= bitmap->dirty_alloc) {
		bitmap->dirty_alloc = bitmap->dirty_alloc ? bitmap->dirty_alloc * 2 : 32;
		bitmap->dirty = realloc(bitmap->dirty, bitmap->dirty_alloc * sizeof(SDL_Rect));
		if (NULL == bitmap->dirty)
			osd_die("realloc() dirty rects");
	}
	
	memcpy(&bitmap->dirty[bitmap->dirty_count], dst, sizeof(*dst));
	r = &bitmap->dirty[bitmap->dirty_count];
	if (r->x < 0) {
		r->w += r->x;
		r->x = 0;
	}
	if (r->y < 0) {
		r->h += r->y;
		r->y = 0;
	}
	if (r->x + r->w > bitmap->surface->w) {
		r->w = bitmap->surface->w - r->x;
	}
	if (r->y + r->h > bitmap->surface->h) {
		r->h = bitmap->surface->h - r->y;
	}
	if (r->w <= 0 || r->h <= 0)
		return;
	if (r->x > bitmap->surface->w || r->y > bitmap->surface->h)
		return;

	bitmap->dirty_count++;
}

#endif	/* !defined(_OSD_BITMAP_H_INCLUDED_) */
