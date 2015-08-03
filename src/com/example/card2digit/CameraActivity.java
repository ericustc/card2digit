package com.example.card2digit;

import android.app.Activity;
import android.hardware.Camera;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

public class CameraActivity extends Activity {
  private CameraPreview mPreview;
  private Camera mCamera;

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);

    requestWindowFeature(Window.FEATURE_NO_TITLE);
    getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);

    setContentView(R.layout.activity_camera);
    mPreview = (CameraPreview) findViewById(R.id.camera);
  }

  @Override
  protected void onResume() {
    super.onResume();
    mCamera = Camera.open();
    mPreview.setCamera(mCamera);
  }

  @Override
  protected void onPause() {
    super.onPause();
    if (mCamera != null) {
      mPreview.setCamera(null);
      mCamera.setPreviewCallback(null);
      mCamera.stopPreview();
      mCamera.release();
      mCamera = null;
    }
  }

}
