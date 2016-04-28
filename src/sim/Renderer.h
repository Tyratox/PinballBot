/*
 * Renderer.h
 *
 * Renders the box2d content
 */

#ifndef SIM_RENDERER_H_
#define SIM_RENDERER_H_

#include <vector>

#include <Box2D/Box2D.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include <SDL2/SDL2_gfxPrimitives.h>

class Renderer : public b2Draw{

	private:

		static const float NUMERATOR;
		static const float DENOMINATOR;

		static const float SCALING;
		static const float PADDING_PERCENT;

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
		int metersToPixels(const float &meters);

		/**
		 * Converts a Box2D vector into screen coordinates using metersToPixels()
		 * @param	position	b2Vec2		The Box2D vector to convert
		 * @return				b2vec2		The converted Box2D vector
		 */
		b2Vec2 toScreenCoords(const b2Vec2 &position);

	public:

		/**
		 * Inits all required values based on the given window width/height and the DPI
		 */
		Renderer(int width, int height);

		/**
		 * Uses SDL functions to remove objects
		 */
		~Renderer();

		/**
		 * Redraws the scene onto the window
		 * @return	void
		 */
		void redraw();

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
		void dPolygon(const b2Vec2* vertices, int32 vertexCount, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, bool filled);

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
		void dCircle(const b2Vec2& center, float32 radius, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, bool filled);

		/**
		 * Implements the b2Draw functions
		 */

		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

		void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
		void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
		void DrawTransform(const b2Transform& xf);

};




#endif /* SIM_RENDERER_H_ */
