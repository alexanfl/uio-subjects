function [] = analyzeSound (y,M)
  y = y(1:M);                         % Cut sound-sample vector to M points.
  g = fft(y)/M;                       % Fourier transform.

  absf2 = abs(g).*abs(g);             % Absolute value of Fourier transform.
  absf2 = absf2(2:M/2);               % Cut away folding and first element.

  f = 1:length(absf2);                % Initiate frequencies.
  Aw = Ra(f);                         % Calculate A-weighting.

  I = sum(absf2)/M                    % Unweighted intensity.
  IA = sum(Aw*absf2)/M                % A-weighted intensity.

  refdB = -155;                       % Reference dB for our device.

  reldB = 10*log10(I)-refdB           % Relative unweighted intensity.

  reldBA = 10*log10(IA)+2.0-refdB     % Relative A-weighted intensity.
