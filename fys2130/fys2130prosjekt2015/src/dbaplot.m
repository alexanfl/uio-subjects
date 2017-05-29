% Program that plots A-weighting.
f = 1:20000;

Aweight = 20*log10(Ra(f(1:20000))) + 2.0;

h = figure;
semilogx(f(1:20000),Aweight, 'LineWidth', 1.0);
set(gca,'LineWidth', 1.0, 'FontSize', 21, 'FontName', 'times');
xlabel('Frekvens [Hz]', 'FontSize', 21, 'FontName', 'times');
ylabel('Vekting [dB]', 'FontSize', 21, 'FontName', 'times');
h_legend = legend('A-vekting');
set(h_legend,'LineWidth', 1.0, 'FontSize', 21, 'FontName', 'times');
grid on;
print -color -depsc '../doc/A-vekting.eps'
