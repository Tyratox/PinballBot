/*
 * Renderer.cpp
 *
 * Renders the box2d content
 */

#ifndef PINBALL_BOT_RENDERER
#define PINBALL_BOT_RENDERER

#include <Box2D/Box2D.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include <SDL2/SDL2_gfxPrimitives.h>

#include <iostream>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
  #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#endif

class Renderer : public b2Draw{

	private:

		const float NUMERATOR			= 7.0f;
		const float DENOMINATOR			= 9.0f;

		const float SCALING				= NUMERATOR / DENOMINATOR;
		const float PADDING_PERCENT		= (DENOMINATOR - NUMERATOR) / DENOMINATOR;

		int		width;
		int		height;

		int		oneMeterInPX;

		SDL_Window*		window;
		SDL_Renderer*	renderer;

		/**
		 * Converts Box2D meters into screen pixels
		 * @param	meters		float		The amount of meters to convert
		 * @return				int			The rounded amount of corresponding screen pixels
		 */
		int metersToPixels(float meters){
			return round(meters * oneMeterInPX);
		}

		/**
		 * Converts a Box2D vector into screen coordinates using metersToPixels()
		 * @param	position	b2Vec2		The Box2D vector to convert
		 * @return				b2vec2		The converted Box2D vector
		 */
		b2Vec2 toScreenCoords(b2Vec2 position){
			return b2Vec2 (
					round(width * PADDING_PERCENT) + metersToPixels(position.x),
					round(height * PADDING_PERCENT) + metersToPixels(position.y)
			);
		}

	public:

		/**
		 * Inits all required values based on the given window width/height and the DPI
		 */
		Renderer(int width, int height) : width(width), height(height){

			SDL_Init( /*SDL_INIT_EVERYTHING*/SDL_INIT_VIDEO );

			window = SDL_CreateWindow(
				"PinballBot",						// window title
				0,									// initial x position
				0,									// initial y position
				this->width,						// width, in pixels
				this->height,						// height, in pixels
				SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI //enables retina support
			);

			if (window == nullptr) {
				printf("Could not create window: %s\n", SDL_GetError());
				SDL_Quit();
				return;
			}

			//updates the width and height if there's a high DPI and calc other vars afterwards
			SDL_GL_GetDrawableSize(window, &width, &height);

			oneMeterInPX = SCALING * height; /* one meter is equal to half of the width of the window */

			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //white background
			SDL_RenderClear(renderer);
		}

		/**
		 * Uses SDL functions to remove objects
		 */
		~Renderer(){
			SDL_DestroyWindow(window);
			SDL_Quit();
		}

		/**
		 * Redraws the scene onto the window
		 * @return	void
		 */
		void redraw(){

			//filledCircleColor(renderer, 320, 91, 10, createRGBA(255,0,0,255));

			SDL_RenderPresent(renderer);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);
		}

		/**
		 * Draws a polygon
		 * @param	vertices		b2Vec2		The vertices of the polygon
		 * @param	vertexCount		int32		The amount of vertices
		 * @param	red				Uint8		The amount of red	in the color
		 * @param	green			Uint8		The amount of green	in the color
		 * @param	blue			Uint8		The amount of blue	in the color
		 * @param	alpha			Uint8		The opacity of the color
		 * @param	filled			bool		Draw a filled polygon or just the outlines?
		 * @return	void
		 */
		void dPolygon(const b2Vec2* vertices, int32 vertexCount, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, bool filled){
			short x[vertexCount];
			short y[vertexCount];

			for(int i=0;i<vertexCount;i++){
				b2Vec2 vec = toScreenCoords(vertices[i]);

				x[i] = vec.x;
				y[i] = vec.y;
			}

			if(filled){
				filledPolygonRGBA(renderer, x, y, vertexCount, red, green, blue, alpha);
			}else{
				polygonRGBA(renderer, x, y, vertexCount, red, green, blue, alpha);
			}
		}

		/**
		 * Draws a circle
		 * @param	center			b2Vec2		The coordinates of the center of the circle
		 * @param	radius			float32		The radius of the circle
		 * @param	red				Uint8		The amount of red	in the color
		 * @param	green			Uint8		The amount of green	in the color
		 * @param	blue			Uint8		The amount of blue	in the color
		 * @param	alpha			Uint8		The opacity of the color
		 * @param	filled			bool		Draw a filled polygon or just the outlines?
		 * @return	void
		 */
		void dCircle(const b2Vec2& center, float32 radius, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, bool filled){
			b2Vec2 coords = toScreenCoords(center);

			if(filled){
				filledCircleRGBA(renderer, coords.x, coords.y, metersToPixels(radius), red, green, blue, alpha);
			}else{
				circleRGBA(renderer, coords.x, coords.y, metersToPixels(radius), red, green, blue, alpha);
			}
		}

		/**
		 * Implements the b2Draw functions
		 */
			void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
				dPolygon(vertices, vertexCount, 0, 0, 0, 255, false);
			}
			void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
				dPolygon(vertices, vertexCount, 0, 0, 0, 255, true);
			}

			void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
				dCircle(center, radius, 0, 0, 0, 255, false);
			}
			void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
				dCircle(center, radius, 0, 0, 0, 255, true);
			}

			void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
				lineRGBA(renderer, p1.x, p1.y, p2.x, p2.y, 0, 0, 0, 255);
			}
			void DrawTransform(const b2Transform& xf) {

			}

};

#endif /* PINBALL_BOT_RENDERER */
