package de.nachregenkommtsonne.airgapclient;

import android.app.Activity;
import android.app.ActionBar;
import android.app.Fragment;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.AudioRecord.OnRecordPositionUpdateListener;
import android.media.MediaRecorder;
import android.media.MediaRecorder.AudioSource;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;
import android.os.Build;



public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (savedInstanceState == null) {
            getFragmentManager().beginTransaction()
                    .add(R.id.container, new PlaceholderFragment())
                    .commit();
        }
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    /**
     * A placeholder fragment containing a simple view.
     */
    public static class PlaceholderFragment extends Fragment {
    	boolean _running = false;
        public PlaceholderFragment() {
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container,
                Bundle savedInstanceState) {
            View rootView = inflater.inflate(R.layout.fragment_main, container, false);
            return rootView;
        }
        
        @Override
        public void onResume() {
        	super.onResume();
        	_running = true;
        	final AudioRecord recorder = new AudioRecord(
					AudioSource.MIC, 44100, AudioFormat.CHANNEL_IN_MONO,
					AudioFormat.ENCODING_PCM_16BIT, AudioRecord.getMinBufferSize(44100, AudioFormat.CHANNEL_IN_MONO,AudioFormat.ENCODING_PCM_16BIT));
			
        	//recorder.
			recorder.setRecordPositionUpdateListener(new OnRecordPositionUpdateListener() {
				
				public void onPeriodicNotification(AudioRecord arg0) {
					// TODO Auto-generated method stub
					Log.e("audio", "onPeriodicNotification");
				}
				
				public void onMarkerReached(AudioRecord arg0) {
					// TODO Auto-generated method stub
					Log.e("audio", "onMarkerReached");
				}
			});
			
			int len = AudioRecord.getMinBufferSize(44100, AudioFormat.CHANNEL_IN_MONO,AudioFormat.ENCODING_PCM_16BIT) * 64;
			recorder.setPositionNotificationPeriod(len);
			recorder.startRecording();
			
			AsyncTask<String, Integer, String> a = new AsyncTask<String, Integer, String>() {
				
				@Override
				protected String doInBackground(String... params) {
					int len = AudioRecord.getMinBufferSize(44100, AudioFormat.CHANNEL_IN_MONO,AudioFormat.ENCODING_PCM_16BIT) * 64;
					short sData[] = new short[len];

					while (_running){
						
				        int numRead = recorder.read(sData, 0, len);
				        
				        
					}

					return "test";
				}
			};
			
			a.execute("test");
			
        	
        }
        
        @Override
        public void onPause() {
        	// TODO Auto-generated method stub
        	super.onPause();
        	_running = false;
        }
    }
}
