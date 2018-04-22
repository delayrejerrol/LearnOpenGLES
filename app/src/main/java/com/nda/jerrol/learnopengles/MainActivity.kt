package com.nda.jerrol.learnopengles

import android.content.res.AssetManager
import android.opengl.GLSurfaceView
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import kotlinx.android.synthetic.main.activity_main.*
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MainActivity : AppCompatActivity() {

    private val renderer = object: GLSurfaceView.Renderer {
        override fun onDrawFrame(gl: GL10?) {
            nativeDrawFrame()
        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
            nativeSurfaceChanged(width, height)
        }

        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            val assetManager = assets
            nativeSurfaceCreated(assetManager)
        }

    }

    private val renderListener = object: RenderListener {
        override fun drawCoin(pieces: Int) {
            nativeDrawCoin(pieces)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        // Set openGL version
        view_main_glsurface.setEGLContextClientVersion(2)
        view_main_glsurface.setRenderer(renderer)

        /*button.setOnClickListener({
            view_main_glsurface.queueEvent ({
                renderListener.drawCoin(editText.text.toString().toInt())
            })
        })*/
    }

    external fun nativeSurfaceCreated(assetManager: AssetManager)
    external fun nativeSurfaceChanged(width: Int, height: Int)
    external fun nativeDrawFrame()
    external fun nativeDrawCoin(pieces: Int)
    companion object {
        init {
            System.loadLibrary("native-bridge")
        }
    }
}
