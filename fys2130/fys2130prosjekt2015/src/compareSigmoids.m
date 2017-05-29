function [] = compareSigmoids (t)
% Function to compare different sigmoids.
  plot(t,sigmoid('arc',t),t,sigmoid('alg3',t),t,sigmoid('tanh',t),t,sigmoid('alg2',t))
  set(gca,'LineWidth', 1.0, 'FontSize', 21, 'FontName', 'times');
  xlabel('Tid [s]', 'FontSize', 21, 'FontName', 'times');
  ylabel('Amplitude', 'FontSize', 21, 'FontName', 'times');
  grid on;
  h_legend=legend('S1', 'S2', 'S3', 'S4');
  set(h_legend,'FontSize', 18, 'Location', 'northwest');
  print -color -depsc '../doc/sigmoidsammenligning.eps' 
endfunction
