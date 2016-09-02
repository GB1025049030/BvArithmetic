package com.azmohan.bvarithmetic;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.bvirtual.BvirtualDemoView;

public class MainActivity extends AppCompatActivity {
    BvirtualDemoView mBvirtual;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mBvirtual = (BvirtualDemoView) findViewById(R.id.bVirtual);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mBvirtual.destroy();
    }
}
