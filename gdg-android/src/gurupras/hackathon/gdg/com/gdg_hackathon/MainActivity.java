package gurupras.hackathon.gdg.com.gdg_hackathon;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.CompoundButton;
import android.widget.Switch;
import gurupras.hackathon.gdg.com.gdg_hackathon.LocalUtils;

public class MainActivity extends Activity {

    public static String TAG = "MainActivity";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        final Switch bd_switch = (Switch) findViewById(R.id.bd_switch);
        final Switch spk_switch = (Switch) findViewById(R.id.spk_switch);
        spk_switch.setChecked(false);
        bd_switch.setChecked(false);
        bd_switch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked) {
                    spk_switch.setChecked(!isChecked);
                    try {
                    LocalUtils.writeToSDCard(LocalUtils.SDCARD + "/" + "hackathon", 1); }
                    catch (Exception e) {
                        Log.e(TAG, e.getMessage());
                    }
                } else {
                    try {
                        LocalUtils.writeToSDCard(LocalUtils.SDCARD + "/" + "hackathon", 0); }
                    catch (Exception e) {
                        Log.e(TAG, e.getMessage());
                    }
                }
            }
        });

        spk_switch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                   if(isChecked) {
                       bd_switch.setChecked(!isChecked);
                       try {
                           LocalUtils.writeToSDCard(LocalUtils.SDCARD + "/" + "hackathon", 2);
                       } catch (Exception e) {
                           Log.e(TAG, e.getMessage());
                       }
                } else {
                   try {
                       LocalUtils.writeToSDCard(LocalUtils.SDCARD + "/" + "hackathon", 0); 
                   } catch (Exception e) {
                       Log.e(TAG, e.getMessage());
                   }
               }
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
