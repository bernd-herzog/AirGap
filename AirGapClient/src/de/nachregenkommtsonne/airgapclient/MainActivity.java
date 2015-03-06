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

	static
	{
		System.loadLibrary("AirGap");
	}
	
	private native String getMessage(short[] samples);

	
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
         	final int len = 44100;
            
         	final AudioRecord recorder = new AudioRecord(
					AudioSource.MIC, 44100, AudioFormat.CHANNEL_IN_MONO,
					AudioFormat.ENCODING_PCM_16BIT, len);
        	
        	//recorder.
			recorder.setRecordPositionUpdateListener(new OnRecordPositionUpdateListener() {
				
				public void onPeriodicNotification(AudioRecord arg0) {
					//Log.e("audio", "onPeriodicNotification");
					//Toast.makeText(getActivity(), "test", Toast.LENGTH_SHORT).show();
					
					//short sData[] = new short[len];
					//int readSamples = recorder.read(sData, 0, len);

					//Log.e("audio", "onPeriodicNotification");
					
					/*
						if (readSamples > 0){
//						Log.e("audio", "onPeriodicNotification");
						AsyncTask<A, Integer, String> a = new AsyncTask<A, Integer, String>() {
							
							protected String doInBackground(A... params) {
								//int len = AudioRecord.getMinBufferSize(44100, AudioFormat.CHANNEL_IN_MONO,AudioFormat.ENCODING_PCM_16BIT);
								short[] asData = params[0].getSamples();

								MainActivity ma = (MainActivity)getActivity();
								
								//String r = ma.messageFromNativeCode(asData);
								
								//Log.e("NDK", r);

								return "test";
							}
						};
						
						A b = new A(sData);
						a.execute(b);

					}*/
				}
				
				public void onMarkerReached(AudioRecord arg0) {
					Toast.makeText(getActivity(), "onMarkerReached", Toast.LENGTH_SHORT).show();

				}
			});

			recorder.setPositionNotificationPeriod(len);
//			recorder.setNotificationMarkerPosition(len);
			recorder.startRecording();
			
			
			AsyncTask<String, Integer, String> a = new AsyncTask<String, Integer, String>() {
				
				protected String doInBackground(String... params) {
					//int len = AudioRecord.getMinBufferSize(44100, AudioFormat.CHANNEL_IN_MONO,AudioFormat.ENCODING_PCM_16BIT);
					//short[] asData = params[0].getSamples();

					//
					
					//
					
					//Log.e("NDK", r);
					MainActivity ma = (MainActivity)getActivity();
					short sData[] = new short[len];

					while (true){
						int readSamples = recorder.read(sData, 0, len);
						//Short[] s = sData;
						Log.e("pre NDK", "" + readSamples);
						String r = ma.getMessage(sData);
						Log.e("NDK", r);
					}
				}
			};
			
			a.execute("");
			
			
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
