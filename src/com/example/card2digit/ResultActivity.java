package com.example.card2digit;

import android.app.Activity;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;

public class ResultActivity extends Activity {

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_result);
    TextView resultView = (TextView) findViewById(R.id.result);
    resultView.setText(getIntent().getStringExtra("text"));
    ImageView imageView = (ImageView) findViewById(R.id.image);
    imageView.setImageBitmap((Bitmap) getIntent().getParcelableExtra("bitmap"));
  }

}
