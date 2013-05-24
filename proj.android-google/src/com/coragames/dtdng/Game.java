/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.coragames.dtdng;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import com.wenbin.ChartboostX.*;
import com.chartboost.sdk.*;

import com.avalon.payment.Backend;
import android.content.Intent;

import android.os.Bundle;

public class Game extends Cocos2dxActivity{
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		ChartboostXBridge.initChartboostXBridge(this);
		Backend.setPublicKey("MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAv/G1iMd4B704VRrx2QuscgW+IrePszT8JdHNWTJtoYTF5c42SeMSP16445psWzNQ6tDaHdNsxOzIArC+jbBYaClk3WWN2PGrp7SgHIGqpb9dCNjjFTdL0Ui3e/NfRP/dMnN4bwFD3Hj/tkbodRMfI991OrRS2RSYTk8H62zxhNQXdvWuZuIxAuegylqZX7EjUZVbTSh+v6LCfFisCI+0wuNyuXeBjPh7Ut+zgWCaFVmnj7bJXLgIKXavEraVGmOA+/2mR71Wt8IE8MLyXpd8Kf2YLxrVlaqnsoiLl7SaDfP2ZHhaBjWb00VxIaEoof+/ojSqmchfxDFZ6bQQsBNJDQIDAQAB");
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// Game should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    @Override 
    protected void onResume()
    {
        super.onResume();
        ChartboostXBridge.initChartboostXBridge(this);
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }     


    @Override
    protected void onStart() {
        super.onStart();
    
        Chartboost.sharedChartboost().onStart(this);
    }

    @Override
    protected void onStop() {
        super.onStop();

        Chartboost.sharedChartboost().onStop(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        Chartboost.sharedChartboost().onDestroy(this);
    }

    @Override
    public void onBackPressed() {
        // If an interstitial is on screen, close it. Otherwise continue as normal.
        if (Chartboost.sharedChartboost().onBackPressed())
            return;
        else
            super.onBackPressed();
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (Backend.handleActivityResult(requestCode, resultCode, data)) {
            super.onActivityResult(requestCode, resultCode, data);
        }
    }
}
