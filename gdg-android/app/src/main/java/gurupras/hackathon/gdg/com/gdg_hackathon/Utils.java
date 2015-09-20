package gurupras.hackathon.gdg.com.gdg_hackathon;

/**
 * Created by Kushal on 20/09/15.
 */

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import android.util.Log;

public class Utils {
    public static final String TAG = MainActivity.TAG + "->" + Utils.class.getSimpleName();

    public static void setProp(String key, Object value) throws IOException {
        Runtime.getRuntime().exec("setprop " + key + " " + value.toString());
        Log.e(TAG, "setting " + key + " prop to" + value.toString());
        Log.e(TAG, "And now it is:"+getProp(key));

    }

    public static String getProp(String key) throws IOException {
        Process p = Runtime.getRuntime().exec("getprop " + key);
        BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));

        String line = null;
        return reader.readLine().trim();
    }
}
