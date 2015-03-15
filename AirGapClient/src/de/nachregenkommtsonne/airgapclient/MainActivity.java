package de.nachregenkommtsonne.airgapclient;

import android.app.Activity;
import android.app.Fragment;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder.AudioSource;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;

public class MainActivity extends Activity {

	static {
		System.loadLibrary("AirGap");
	}

	private native String getMessage(short[] samples);

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		if (savedInstanceState == null) {
			getFragmentManager().beginTransaction()
					.add(R.id.container, new PlaceholderFragment()).commit();
		}
	}

	public static class PlaceholderFragment extends Fragment {
		boolean _running = false;
		TextView textView;
		ScrollView scrollView;
		ProgressBar progressBar;
		SeekBar seekBar;
		
		int maxValue = 0;
		float amp = 1.0f;

		public PlaceholderFragment() {
			
		}

		public View onCreateView(LayoutInflater inflater, ViewGroup container,
				Bundle savedInstanceState) {
			View rootView = inflater.inflate(R.layout.fragment_main, container,
					false);

			textView = (TextView) rootView.findViewById(R.id.textView1);
			scrollView = (ScrollView) rootView.findViewById(R.id.scroll);

			progressBar = (ProgressBar) rootView
					.findViewById(R.id.progressBar1);
			seekBar = (SeekBar) rootView.findViewById(R.id.seekBar1);
			seekBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {

				public void onStopTrackingTouch(SeekBar seekBar) {
				}

				public void onStartTrackingTouch(SeekBar seekBar) {
				}

				public void onProgressChanged(SeekBar seekBar, int progress,
						boolean fromUser) {
					amp = 1.0f + progress ;
				}
			});
			return rootView;
		}

		public void onResume() {
			super.onResume();
			_running = true;
			final int len = 4410;

			final AudioRecord recorder = new AudioRecord(AudioSource.MIC,
					44100, AudioFormat.CHANNEL_IN_MONO,
					AudioFormat.ENCODING_PCM_16BIT, len * 2);

			recorder.startRecording();

			AsyncTask<String, String, String> a = new AsyncTask<String, String, String>() {

				protected String doInBackground(String... params) {
					MainActivity ma = (MainActivity) getActivity();
					short sData[] = new short[len];

					while (_running) {
						int readSamples = recorder.read(sData, 0, len);

						int max = 0;

						for (int i = 0; i < readSamples; i++) {
							sData[i] = (short) ((float) sData[i] * amp);

							int val = Math.abs(sData[i]);
							if (val > max)
								max = val;
						}
						maxValue = max;

						// Log.e("pre NDK", "" + readSamples);
						String r = ma.getMessage(sData);
						
						r = r.replace("\r", "\n");
						
						// if (r != null && r.length() > 0){
						this.publishProgress(r);
						// }
					}

					return "";
				}

				@Override
				protected void onProgressUpdate(String... values) {

					progressBar.setProgress(maxValue);

					if (values[0] == null || values[0].length() == 0)
						return;

					textView.setText(values[0]);
					//scrollView.fullScroll(View.FOCUS_DOWN);
				}
			};

			a.execute("");
		}

		public void onPause() {
			super.onPause();
			_running = false;
		}
	}
}
