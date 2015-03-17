# Yam2D - Yet Another Mobile 2D Game Engine #

Simple 2D game engine, mainly targetted for mobile devices. Oparates also on PC in OpenGL ES emulation.

Yam2d is released under MIT-license. See: http://opensource.org/licenses/mit-license.php

## Currently supported platforms: ##
  * Windows (x86 compatible)
  * Android

## Current key features: ##
  * Textures
    * Textures can be loaded from png-file.

  * Low-level drawing capabilities
    * Batched 2D-sprite drawing.
    * Batched 2D-text drawing using bitmap fonts.
    * OpenGL ES 1.X functionality can be also used to draw primitives

  * High level scene/level functionality
    * Maps
      * Orthogonal
      * Diamond isomertic
      * Map loading from tmx-files
    * Layers
    * Ready made GameObjects
      * Sprites
      * Tiles
      * Animated sprites
      * Texts
    * Simple pick-functionality

## Input: ##
  * Keyboard
  * Mouse
  * XBOX Controller (Windows only)

## Tutorials: ##
  * TemplateProject
  * LowLevelDraw
  * SimpleMap
  * TmxMapLoading
  * CustomMapCreation
  * ObjectMoveAndMouseLookAt
  * SpriteAnimation
  * OwnGameObjectTypes

## Tools: ##
  * TmxMapViewer

## Documentation: ##
  * Instructions to download yam2d and make environment setup https://github.com/Miceroy/yam2d/blob/master/doc/DownloadAndEnvironmentSetup.docx
  * Instructions to create new project https://github.com/Miceroy/yam2d/blob/master/doc/CreateNewProject.docx
  * Instructions to integrating TmxMapViewer-tool with Tiled Editor https://github.com/Miceroy/yam2d/blob/master/doc/IntegratingTmxMapViewerWithTiled.docx

## External libraries used by yam2d: ##
  * OpenGL ES Emulation libraries
  * libpng
  * ticpp
  * zlib
  * tmx-parser
  * Box2D
  * slmath
  * enet
