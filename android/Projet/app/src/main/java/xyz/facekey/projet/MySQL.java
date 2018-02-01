package xyz.facekey.projet;


import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.util.Base64;
import android.util.Log;
import android.widget.Toast;

import java.io.BufferedWriter;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FilenameFilter;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;

public class MySQL extends Service {
    Bitmap bitmap;

    private static final String TAG = "Service mySQL";
    private boolean isRunning  = false;

    @Override
    public void onCreate() {
        Log.i(TAG, "Service onCreate");
        isRunning = true;
    }


    private Context appContext;

    // Function to toast on main thread
    void showToast(final String toShow){
        // checking app context
        if(null !=appContext){
            // geting main theard
            Handler handler = new Handler(Looper.getMainLooper());
            handler.post(new Runnable() {
                @Override
                public void run() {
                    // threading
                    Toast.makeText(appContext, toShow, Toast.LENGTH_SHORT).show();
                }
            });
        }
    }


    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        // getting app base context
        appContext=getBaseContext();
        new Thread(new Runnable() {
            @Override
            public void run() {
                // Accessing custom pictures folder
                String root = Environment.getExternalStorageDirectory().getAbsolutePath();
                File myDir = new File(root + "/saved_images");
                // Listing files
                File[] filelist = myDir.listFiles(IMAGE_FILTER);
                // browsing files
                int i = 0;
                for (File f : filelist) {
                    // Sleeping 0.01s between each files
                    try {
                        Thread.sleep(10);
                    } catch (Exception e) {
                        Log.e("Error", "exception");
                    }
                    if (isRunning) {
                        Log.d("Files", "FileName:" + f.getName());
                        // getting file path
                        String filePath = f.getPath();
                        // converting to bitmap
                        bitmap = BitmapFactory.decodeFile(filePath);
                        // starting upload fonction
                        uploadImage();
                        // notify user
                        String toToast = "Uploading" + (i+1) + "/" + filelist.length + "...";
                        showToast(toToast);
                        // deleting file
                        boolean del = f.delete();
                        Log.d("Files", "deleted :" + del);
                        Log.i(TAG, "Service running");
                        i++;
                    }
                }
                stopSelf();
            }
        }).start();
        return Service.START_STICKY;
    }
    @Override
    public IBinder onBind(Intent arg0) {
        Log.i(TAG, "Service onBind");
        return null;
    }
    @Override
    public void onDestroy() {
        isRunning = false;
        //notify User
        showToast("All file uploaded");
        Log.i(TAG, "Service onDestroy");
    }


    // URL
    public static final String UPLOAD_URL = "http://www.louislharidon.fr/projet_L2_S4/connexion.php";


    // Files extensions
    static final String[] EXTENSIONS = new String[]{
            "gif", "png", "bmp" // and other formats you need
    };
    // Getting filtered with file extension
    static final FilenameFilter IMAGE_FILTER = new FilenameFilter() {
        @Override
        public boolean accept(final File dir, final String name) {
            for (final String ext : EXTENSIONS) {
                if (name.endsWith("." + ext)) {
                    return (true);
                }
            }
            return (false);
        }
    };


    // Converting bitmap to base64 function
    public String getStringImage(Bitmap bmp){
        // new array
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        bmp.compress(Bitmap.CompressFormat.JPEG, 100, baos);
        byte[] imageBytes = baos.toByteArray();
        // return converted to base64 array
        return Base64.encodeToString(imageBytes, Base64.DEFAULT);
    }

    //upload image fonction
    private void uploadImage() {
        Log.d("Updload", "uploading ");
        // converting to base64
        String uploadImage = getStringImage(bitmap);
        // Posting to PHP
        POST(UPLOAD_URL, uploadImage);
    }

    // Posting to PHP function
    public void POST(String... params){
        String urlString = params[0]; // URL
        String data = params[1]; //data  POST
        try {
            // Opening connexion
            URL url = new URL(urlString);
            // URL
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setReadTimeout(10000);
            conn.setConnectTimeout(15000);
            // POST METHOD
            conn.setRequestMethod("POST");
            conn.setDoInput(true);
            conn.setDoOutput(true);

            // Building request  with url and data
            Uri.Builder builder = new Uri.Builder().appendQueryParameter("image", data);
            Log.d("Updload", "POST : " + builder.toString());
            String query = builder.build().getEncodedQuery();
            // outputstream on connexion
            OutputStream os = conn.getOutputStream();
            // Buffered writer to write on outputsteam
            BufferedWriter writer = new BufferedWriter(
                    new OutputStreamWriter(os, "UTF-8"));
            writer.write(query);
            writer.flush();
            writer.close();
            os.close();
            Log.d("Updload", "url : " + conn.getURL().toString());
            // Request
            conn.connect();
        }catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
}



