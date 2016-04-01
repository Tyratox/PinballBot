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

		int		width;
		int		height;

		int		oneMeterInPX;

		SDL_Window*		window;
		SDL_Renderer*	renderer;

		unsigned long createRGBA(int r, int g, int b, int a){
		    return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
		           + (a & 0xff);
		}

		unsigned long b2ColorToRGBA(b2Color color){
			return createRGBA(color.r, color.g, color.b, color.a);
		}

		int metersToPixels(float meters){
			return round(meters * oneMeterInPX);
		}

		std::pair<int,int> toScreenCoords(b2Vec2 position){
			return std::pair< int, int > (
					metersToPixels(position.x),
					metersToPixels(position.y)
			);
		}

	public:

		Renderer(int width, int height) : width(width), height(height){

			SDL_Init( /*SDL_INIT_EVERYTHING*/SDL_INIT_VIDEO );

			window = SDL_CreateWindow(
				"PinballBot",						// window title
				0,									// initial x position
				0,									// initial y position
				this->width,						// width, in pixels
				this->height,						// height, in pixels
				SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI /*retina support*/
			);

			if (window == nullptr) {
				printf("Could not create window: %s\n", SDL_GetError());
				SDL_Quit();
				return;
			}

			//updates the width and height if there's a high DPI and calc other vars afterwards
			SDL_GL_GetDrawableSize(window, &width, &height);

			oneMeterInPX = width/2; /* one meter is equal to half of the width of the window */

			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //white background
			SDL_RenderClear(renderer);
		}

		~Renderer(){
			SDL_DestroyWindow(window);
			SDL_Quit();
		}

		void redraw(){

			//filledCircleColor(renderer, 320, 91, 10, createRGBA(255,0,0,255));

			SDL_RenderPresent(renderer);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);
		}

		void dPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color){
			short x[vertexCount];
			short y[vertexCount];

			//std::cout << "Polygon with " << vertexCount << " vertices";

			for(int i=0;i<vertexCount;i++){
				x[i] = metersToPixels(vertices[i].x);
				y[i] = metersToPixels(vertices[i].y);

				//std::cout << "Vert: " << x[i] << "|" << y[i] << std::endl;
			}

			polygonRGBA(renderer, x, y, vertexCount, 255, 0, 0, 255);
		}

		void dCircle(const b2Vec2& center, float32 radius, const b2Color& color){
			std::pair<int,int> coords = toScreenCoords(center);
			//std::cout << "draw circle at " << coords.first << "|" << center.x << "||" << coords.second << "|" << center.y << " and radius " <<  metersToPixels(radius) << "(" << radius << ")" << std::endl;
			filledCircleColor(renderer, coords.first, coords.second, metersToPixels(radius), createRGBA(255,0,0,255));
		}

		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
			dPolygon(vertices, vertexCount, color);
		}
		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
			dPolygon(vertices, vertexCount, color);
		}

		void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
			dCircle(center, radius, color);
		}
		void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
			dCircle(center, radius, color);
		}

		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {

		}
		void DrawTransform(const b2Transform& xf) {

		}

};

#endif /* PINBALL_BOT_RENDERER */
