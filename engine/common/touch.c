/*
touchscreen.c - touchscreen support prototype
Copyright (C) 2015 a1batross

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "common.h"
#include "gl_local.h"
#include "input.h"
#include "client.h"

typedef enum
{
	touch_tap,		// Just tap a button
	touch_stick		// Like a joystick stick.
} touchButtonType;

typedef enum
{
	event_down = 0,
	event_up,
	event_motion
} touchEventType;

typedef struct touchbutton_s
{
	// Touch button type: tap, stick or slider
	touchButtonType buttonType;
	// Field of button in pixels
	float left, right, top, bottom, alpha;
	// Button texture
	int texture;
	char command[256];
	// Next
	struct touchbutton_s *next;
} touchbutton_t;

struct touch_s
{
	void *mempool;
	touchbutton_t *first;
} touch;

void IN_AddButton( const char *texture, char *command, float left, float right, float top, float bottom )
{
	touchbutton_t *button = Mem_Alloc( touch.mempool, sizeof( touchbutton_t ) );
	MsgDev( D_NOTE, "IN_AddButton()\n");
	button->texture = GL_LoadTexture( texture, NULL, 0, TF_IMAGE, NULL ); // WHY  it retorn 0, not 2?
	button->left = left;
	button->top = top;
	button->right = right;
	button->bottom = bottom;
	Q_strncpy( button->command, command, sizeof( button->command ) );
	button->next = touch.first;
	touch.first = button;
}

void IN_TouchInit( void )
{
	touch.mempool = Mem_AllocPool( "Touch" );
	touch.first = NULL;
	MsgDev( D_NOTE, "IN_TouchInit()\n");
	IN_AddButton( "*white", "+jump", 0.1, 0.2, 0.1, 0.2 );
}

void IN_TouchDraw( void )
{
	touchbutton_t *button;
	GL_SetRenderMode( kRenderNormal );
	for( button = touch.first; button; button = button->next)
	{
		
		//MsgDev( D_NOTE, "IN_TouchDraw : %d\n", button->texture );
		R_DrawStretchPic( scr_width->integer * button->left,
			scr_height->integer * button->top,
			scr_width->integer * (button->right - button->left),
			scr_height->integer * (button->bottom - button->top),
			0, 0, 1, 1, button->texture);
	}
}
int IN_TouchEvent( touchEventType type, int fingerID, float x, float y, float dx, float dy )
{
	touchbutton_t *button;
	// Find suitable button
	MsgDev( D_NOTE, "IN_TouchEvent( %d, %d, %f, %f, %f, %f )\n", type, fingerID, x, y, dx, dy );
	for( button = touch.first; button  ; button = button->next )
	{
		if( ( x > button->left &&
			 x < button->right ) &&
			( y < button->bottom &&
			  y > button->top ) )
		{
			MsgDev( D_NOTE, "Command: %s\n", button->command );
			Cbuf_AddText( button->command );
			break;
		}
	}
	return 0;
}
