package de.nachregenkommtsonne.airgapclient;

import android.app.Activity;
import android.app.Fragment;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.AudioRecord.OnRecordPositionUpdateListener;
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
        	final int len = 4410;
        	//recorder.
			recorder.setRecordPositionUpdateListener(new OnRecordPositionUpdateListener() {
				
				public void onPeriodicNotification(AudioRecord arg0) {
					
					short sData[] = new short[len];
					int readSamples = recorder.read(sData, 0, len);
					if (readSamples > 0){
						Log.e("audio", "onPeriodicNotification");
						AsyncTask<A, Integer, String> a = new AsyncTask<A, Integer, String>() {
							
							protected String doInBackground(A... params) {
								//int len = AudioRecord.getMinBufferSize(44100, AudioFormat.CHANNEL_IN_MONO,AudioFormat.ENCODING_PCM_16BIT);
								short[] asData = params[0].getSamples();

								//while (_running){
									
							        
							        
							        
								//}

								return "test";
							}
						};
						
						A b = new A(sData);
						a.execute(b);

					}
				}
				
				public void onMarkerReached(AudioRecord arg0) {}
			});

			recorder.setPositionNotificationPeriod(len);
			recorder.startRecording();
        }
        
        public void onPause() {
        	super.onPause();
        	_running = false;
        }
        
        
        class A{
        	short[] samples;

			public short[] getSamples() {
				return samples;
			}

			public A(short[] samples) {
				super();
				this.samples = samples;
			}

       	
        }
    }
}
