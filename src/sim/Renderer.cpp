/*
 * Renderer.cpp
 *
 * Renders the box2d content
 */

#include <vector>
#include <stdio.h>

#include <Box2D/Box2D.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "Renderer.h"
#include "UserData.h"

const float Renderer::NUMERATOR				= 7.0f;
const float Renderer::DENOMINATOR			= 9.0f;

const float Renderer::SCALING				= NUMERATOR / DENOMINATOR;
const float Renderer::PADDING_PERCENT		= (DENOMINATOR - NUMERATOR) / DENOMINATOR / 2;

int Renderer::metersToPixels(const float &meters){
	return round(meters * oneMeterInPX);
}

b2Vec2 Renderer::toScreenCoords(const b2Vec2 &position){
	return b2Vec2 (
			round(width * PADDING_PERCENT) + metersToPixels(position.x),
			round(height * PADDING_PERCENT) + metersToPixels(position.y)
	);
}

Renderer::Renderer(int width, int height, const b2World *world) : width(width), height(height), world(world){

	SDL_Init( SDL_INIT_VIDEO );

	window = SDL_CreateWindow(
		"PinballBot",						// window title
		SDL_WINDOWPOS_CENTERED,				// initial x position
		SDL_WINDOWPOS_CENTERED,				// initial y position
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
	SDL_GL_GetDrawableSize(window, &this->width, &this->height);

	oneMeterInPX = round(SCALING * this->height); /* one meter is equal to the height */

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //white background
	SDL_RenderClear(renderer);
}

Renderer::~Renderer(){
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Renderer::render(){

	for(const b2Body *body = this->world->GetBodyList(); body; body = body->GetNext()){

		UserData *userData = (UserData*) body->GetUserData();
		if(!userData){
			printf("DIDN'T DRAW! Body Position: X(%f), Y(%f); Type: N/A\n", body->GetPosition().x, body->GetPosition().y);
		}

		for(const b2Fixture *fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()){

			b2Shape::Type shapeType = fixture->GetType();

			if (shapeType == b2Shape::e_circle ){
			    b2CircleShape* circleShape = (b2CircleShape*)fixture->GetShape();

			    this->drawCircle(body->GetPosition(), circleShape->m_radius, userData->red, userData->green, userData->blue, userData->alpha, userData->filled);
			}else if (shapeType == b2Shape::e_polygon ){
			    b2PolygonShape* polygonShape = (b2PolygonShape*)fixture->GetShape();

			    const b2Vec2 *vertices_orig = polygonShape->m_vertices;

			    std::vector<b2Vec2> vertices(polygonShape->GetVertexCount());
			    for(int i=0;i < polygonShape->GetVertexCount();i++){
			    	vertices[i] = body->GetWorldPoint(vertices_orig[i]);
			    }

			    this->drawPolygon(vertices.data(), polygonShape->GetVertexCount(), userData->red, userData->green, userData->blue, userData->alpha, userData->filled);
			}else if(shapeType == b2Shape::e_edge ){
				 b2EdgeShape* edgeShape = (b2EdgeShape*)fixture->GetShape();

				 drawLine(body->GetWorldPoint(edgeShape->m_vertex1), body->GetWorldPoint(edgeShape->m_vertex2), userData->red, userData->green, userData->blue, userData->alpha);
			}else if(shapeType == b2Shape::e_chain){
				 b2ChainShape* chainShape = (b2ChainShape*)fixture->GetShape();

				 for(int j=0;j < (chainShape->m_count - 1);j++){
					 drawLine(body->GetWorldPoint(chainShape->m_vertices[j]), body->GetWorldPoint(chainShape->m_vertices[j+1]), userData->red, userData->green, userData->blue, userData->alpha);
				 }
			}else{
				printf("Unknown shapeType: %d!", shapeType);
			}
		}
	}

	this->redraw();
}

void Renderer::redraw(){

	SDL_RenderPresent(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
}

void Renderer::drawLine(const b2Vec2& p1, const b2Vec2& p2, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha){
	b2Vec2 from = toScreenCoords(p1);
	b2Vec2 to = toScreenCoords(p2);

	lineRGBA(renderer, (Sint16) from.x, (Sint16) from.y, (Sint16) to.x, (Sint16) to.y, red, green, blue, alpha);
}

void Renderer::drawPolygon(const b2Vec2* vertices, int32 vertexCount, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, bool filled){
	std::vector<short> x(vertexCount);
	std::vector<short> y(vertexCount);

	for(int i=0;i<vertexCount;i++){
		b2Vec2 vec = toScreenCoords(vertices[i]);

		x[i] = (short) vec.x;
		y[i] = (short) vec.y;
	}

	if(filled){
		filledPolygonRGBA(renderer, x.data(), y.data(), vertexCount, red, green, blue, alpha);
	}else{
		polygonRGBA(renderer, x.data(), y.data(), vertexCount, red, green, blue, alpha);
	}
}

void Renderer::drawCircle(const b2Vec2& center, float32 radius, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, bool filled){
	b2Vec2 coords = toScreenCoords(center);

	if(filled){
		filledCircleRGBA(renderer, (Sint16) coords.x, (Sint16) coords.y, (Sint16) metersToPixels(radius), red, green, blue, alpha);
	}else{
		circleRGBA(renderer, (Sint16) coords.x, (Sint16) coords.y, (Sint16) metersToPixels(radius), red, green, blue, alpha);
	}
}
