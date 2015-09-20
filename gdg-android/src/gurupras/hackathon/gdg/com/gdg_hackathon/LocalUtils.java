package gurupras.hackathon.gdg.com.gdg_hackathon;

/**
 * Created by Kushal on 20/09/15.
 */

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;

import edu.buffalo.cse.phonelab.toolkit.android.utils.Utils;
import android.os.Environment;
import android.util.Log;

public class LocalUtils {
    public static final String TAG = MainActivity.TAG + "->" + Utils.class.getSimpleName();
    public static final String HACK_PATH = Environment.getExternalStorageDirectory().getPath();
    //public static final String HACK_PATH = "/mnt/hack";
    
    public static void setProp(final String key, final Object value) throws InterruptedException, Exception {
	        	Process p = Runtime.getRuntime().exec("su /system/bin/setprop " + key + " " + value.toString());
        		p.waitFor();
    	        BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
    	        String line = null;
				while((line = reader.readLine()) != null)
					Log.d(TAG, line);
				reader = new BufferedReader(new InputStreamReader(p.getErrorStream()));
    	        line = null;
				while((line = reader.readLine()) != null)
					Log.e(TAG, line);
    	        Log.e(TAG, "setting " + key + " prop to" + value.toString());
				Log.e(TAG, "And now it is:"+getProp(key));
    }

    public static String getProp(String key) throws IOException, InterruptedException {
        Process p = Runtime.getRuntime().exec("/system/bin/getprop " + key);
        p.waitFor();
        BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));

        return reader.readLine().trim();
    }
    
    public static void writeToSDCard(String file, int val) throws IOException {
    	File f = new File(file);
    	FileWriter writer = new FileWriter(f);
    	writer.write("" + val);
    	writer.close();
    }
}
