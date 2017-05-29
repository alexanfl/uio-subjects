Fs = 44100;                         % Sampling frequency.
recT = 3;                           % Record time.

y = recordSound(recT,Fs);           % Record sound.

wavwrite(y,Fs,16,'test.wav');       % Save file in .wav format.

M = 2^17;                           % Number of samples we want.

analyzeSound(y,M)                   % Compute sound analysis.
