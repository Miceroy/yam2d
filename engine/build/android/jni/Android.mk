LOCAL_PATH:= $(call my-dir)

ENGINE_PATH := $(LOCAL_PATH)../../../..
ENGINE_SRC_PATH := ../../../source
ENGINE_EXT_SRC_PATH := ../../../external

include $(CLEAR_VARS)

LOCAL_MODULE    := yam2d

LOCAL_CFLAGS := -D_STLP_USE_SIMPLE_NODE_ALLOC -DANDROID -frtti -fexceptions

LOCAL_C_INCLUDES := \
	$(ENGINE_PATH)/include \
	$(ENGINE_PATH)/external


LOCAL_SRC_FILES := \
	$(ENGINE_SRC_PATH)/android/es_util_android.cpp \
	$(ENGINE_SRC_PATH)/android/input_android.cpp \
	$(ENGINE_SRC_PATH)/win32/es_util_png.cpp \
	$(ENGINE_SRC_PATH)/FileStream.cpp \
	$(ENGINE_SRC_PATH)/AnimationTimeline.cpp \
	$(ENGINE_SRC_PATH)/ElapsedTimer.cpp \
	$(ENGINE_SRC_PATH)/GameObject.cpp \
	$(ENGINE_SRC_PATH)/Layer.cpp \
	$(ENGINE_SRC_PATH)/Map.cpp \
	$(ENGINE_SRC_PATH)/MapTile.cpp \
	$(ENGINE_SRC_PATH)/Object.cpp \
	$(ENGINE_SRC_PATH)/PropertySet.cpp \
	$(ENGINE_SRC_PATH)/Sprite.cpp \
	$(ENGINE_SRC_PATH)/SpriteAnimation.cpp \
	$(ENGINE_SRC_PATH)/SpriteBatch.cpp \
	$(ENGINE_SRC_PATH)/SpriteSheet.cpp \
	$(ENGINE_SRC_PATH)/Text.cpp \
	$(ENGINE_SRC_PATH)/Texture.cpp \
	$(ENGINE_SRC_PATH)/Tileset.cpp \
	$(ENGINE_SRC_PATH)/es_util.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Collision/b2BroadPhase.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Collision/b2CollideCircle.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Collision/b2CollideEdge.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Collision/b2CollidePolygon.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Collision/b2Collision.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Collision/b2Distance.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Collision/b2DynamicTree.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Collision/b2TimeOfImpact.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Collision/Shapes/b2ChainShape.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Collision/Shapes/b2CircleShape.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Collision/Shapes/b2EdgeShape.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Collision/Shapes/b2PolygonShape.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Common/b2BlockAllocator.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Common/b2Draw.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Common/b2Math.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Common/b2Settings.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Common/b2StackAllocator.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Common/b2Timer.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/b2Body.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/b2ContactManager.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/b2Fixture.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/b2Island.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/b2World.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/b2WorldCallbacks.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Contacts/b2Contact.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Joints/b2GearJoint.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Joints/b2Joint.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Joints/b2RopeJoint.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Joints/b2WeldJoint.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Dynamics/Joints/b2WheelJoint.cpp \
	$(ENGINE_EXT_SRC_PATH)/Box2D/Rope/b2Rope.cpp \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/png.c \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/pngerror.c \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/pngget.c \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/pngmem.c \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/pngpread.c \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/pngread.c \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/pngrio.c \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/pngrtran.c \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/pngrutil.c \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/pngset.c \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/pngtrans.c \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/pngwio.c \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/pngwrite.c \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/pngwtran.c \
	$(ENGINE_EXT_SRC_PATH)/lpng1513/pngwutil.c \
	$(ENGINE_EXT_SRC_PATH)/ticpp/ticpp.cpp \
	$(ENGINE_EXT_SRC_PATH)/ticpp/tinystr.cpp \
	$(ENGINE_EXT_SRC_PATH)/ticpp/tinyxml.cpp \
	$(ENGINE_EXT_SRC_PATH)/ticpp/tinyxmlerror.cpp \
	$(ENGINE_EXT_SRC_PATH)/ticpp/tinyxmlparser.cpp \
	$(ENGINE_EXT_SRC_PATH)/tmx-parser/base64/base64.cpp \
	$(ENGINE_EXT_SRC_PATH)/tmx-parser/TmxImage.cpp \
	$(ENGINE_EXT_SRC_PATH)/tmx-parser/TmxLayer.cpp \
	$(ENGINE_EXT_SRC_PATH)/tmx-parser/TmxMap.cpp \
	$(ENGINE_EXT_SRC_PATH)/tmx-parser/TmxObject.cpp \
	$(ENGINE_EXT_SRC_PATH)/tmx-parser/TmxObjectGroup.cpp \
	$(ENGINE_EXT_SRC_PATH)/tmx-parser/TmxPolygon.cpp \
	$(ENGINE_EXT_SRC_PATH)/tmx-parser/TmxPropertySet.cpp \
	$(ENGINE_EXT_SRC_PATH)/tmx-parser/TmxTile.cpp \
	$(ENGINE_EXT_SRC_PATH)/tmx-parser/TmxTileset.cpp \
	$(ENGINE_EXT_SRC_PATH)/tmx-parser/TmxUtil.cpp \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/adler32.c \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/compress.c \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/crc32.c \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/deflate.c \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/gzclose.c \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/gzlib.c \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/gzread.c \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/gzwrite.c \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/infback.c \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/inffast.c \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/inflate.c \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/inftrees.c \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/trees.c \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/uncompr.c \
	$(ENGINE_EXT_SRC_PATH)/zlib-1.2.7/zutil.c

LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_STATIC_LIBRARY)


$(call import-module,android/native_app_glue)



