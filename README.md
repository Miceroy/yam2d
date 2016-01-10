#Yam2D - Yet Another Mobile 2D Game Engine

Simple 2D game engine, mainly targetted for mobile devices. Oparates also on PC in OpenGL ES emulation.

Currently Yam2d has only win32 version but Android version is coming at summer 2013.

##Currently supported platforms:
  * Windows (x86 compatible)
  * Android

##Current key features:
  * Textures
    * Textures can be loaded from png-file.

  * Low-level drawing capabilities
    * Batched 2D-sprite drawing.
    * Batched 2D-text drawing using bitmap fonts.
    * OpenGL ES 1.X functionality can be also used to draw primitives

  * High level scene/level functionality using Entity-Component Architecture
    * Maps
      * Orthogonal
      * Diamond isomertic
      * Map loading from tmx-files
    * Layers
    * Supported Components
      * Sprite
      * Tile (Tilemap tile)
      * AnimatedSprites
      * Text
    * Simple pick-functionality
 
  * Animation framework

##Input:
  * Keyboard
  * Mouse
  * XBOX Controller (Windows only)

##Tutorials:
  * TemplateProject
  * LowLevelDraw
  * SimpleMap
  * TmxMapLoading
  * CustomMapCreation
  * ObjectMoveAndMouseLookAt
  * SpriteAnimation
  * OwnGameObjectTypes

##Tools:
  * TmxMapViewer

##Documentation:
  * [https://yam2d.googlecode.com/svn/trunk/doc/DownloadAndEnvironmentSetup.docx Instructions to download yam2d and make environment setup]
  * [https://yam2d.googlecode.com/svn/trunk/doc/CreateNewProject.docx Instructions to create new project] 
  * [https://yam2d.googlecode.com/svn/trunk/doc/IntegratingTmxMapViewerWithTiled.docx Instructions to integrating TmxMapViewer-tool with Tiled Editor]

##External libraries used by yam2d:
  * OpenGL ES Emulation libraries
  * libpng
  * ticpp
  * zlib
  * tmx-parser
  * Box2D
  * slmath
  * enet
