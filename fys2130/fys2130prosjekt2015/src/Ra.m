function [Aw] = Ra (frequency)
% This function returns a weight for the dB(A) scale.
  f = frequency;
  f2 = f.*f;        % Efficient vectorized calculations of f^2 and f^4.
  f4 = f2.*f2;
  Aw = (12200^2*f4)./((f2+20.6^2).*sqrt((f2+107.7^2).*(f2+737.9^2)).*(f2+12200^2));
endfunction
