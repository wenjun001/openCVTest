package com.example.wma.opencvtest;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;


public class MainActivity extends  AppCompatActivity implements View.OnClickListener {
    private ImageView imageView;

    private static final String FILENAME = "temp_file.txt";

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.i("test","-------------------onCreate-------------------------");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        imageView = findViewById(R.id.imageView);
        findViewById(R.id.show).setOnClickListener(this);
        findViewById(R.id.process).setOnClickListener(this);
        File sdcard = Environment.getExternalStorageDirectory();

        Log.i("test", "--------onCreate: "+sdcard.getAbsolutePath());
    }


    private void startServerSocket() {

        Thread thread = new Thread(new Runnable() {

            private String stringData = null;

            @Override
            public void run() {

                byte[] msg = new byte[1000];
                DatagramPacket dp = new DatagramPacket(msg, msg.length);
                DatagramSocket ds = null;
                try {
                    ds = new DatagramSocket(9234);
                    ds.receive(dp);

                    stringData = new String(msg, 0, dp.getLength());
                    Log.d("TEST", "run: "+stringData);

//                    String msgToSender = "Bye Bye ";
//                    dp = new DatagramPacket(msgToSender.getBytes(), msgToSender.length(), dp.getAddress(), dp.getPort());
//                    ds.send(dp);

                } catch (IOException e) {
                    e.printStackTrace();
                } finally {
                    if (ds != null) {
                        ds.close();
                    }
                }
            }

        });
        thread.start();
    }


    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.show) {

            File file = new File(Environment.getExternalStorageDirectory(),
                    FILENAME);

//            startServerSocket();

//
//            if (Environment.getExternalStorageState().equals(
//                    Environment.MEDIA_MOUNTED)) {
//
//                try {
//                    FileOutputStream fos = new FileOutputStream(file);
//                    fos.write("test test test".getBytes());
//                    fos.close();
//                    Toast.makeText(MainActivity.this, "写入文件成功",
//                            Toast.LENGTH_LONG).show();
//                } catch (Exception e) {
//                    e.printStackTrace();
//                    Toast.makeText(MainActivity.this, "写入文件失败",
//                            Toast.LENGTH_SHORT).show();
//                }
//            } else {
//
//                Toast.makeText(MainActivity.this,
//                        "此时SDcard不存在或者不能进行读写操作", Toast.LENGTH_SHORT).show();
//            }




//
//            if (Environment.getExternalStorageState().equals(
//                    Environment.MEDIA_MOUNTED)) {
//                try {
//                    FileInputStream inputStream = new FileInputStream(file);
//                    byte[] b = new byte[inputStream.available()];
//
//                    Toast.makeText(MainActivity.this, "读取文件成功",
//                            Toast.LENGTH_LONG).show();
//                } catch (Exception e) {
//                    Toast.makeText(MainActivity.this, "读取失败",
//                            Toast.LENGTH_SHORT).show();
//                }
//            } else {
//                // 此时SDcard不存在或者不能进行读写操作的
//                Toast.makeText(MainActivity.this,
//                        "此时SDcard不存在或者不能进行读写操作", Toast.LENGTH_SHORT).show();
//            }






            Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.test);
            imageView.setImageBitmap(bitmap);
        } else {
            Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.test);
            getEdge(bitmap);
            imageView.setImageBitmap(bitmap);
        }
    }


    native void getEdge(Object bitmap);
}
