import sys
import csv
import plotly.graph_objects as go

ARQUIVO = "resultados.txt"

# Leitura do arquivo CSV gerado pelo benchmark.sh
tamanhos = []
tempos = []
with open(ARQUIVO, newline="") as arquivo_csv:
    leitor = csv.DictReader(arquivo_csv)
    for linha in leitor:
        tamanhos.append(int(linha["tamanho"]))
        tempos.append(float(linha["tempo_segundos"]))

# Detecta pontos onde o tempo dobra
tamanhos_dobra = []
tempos_dobra = []
tempo_referencia = tempos[0]
for i in range(1, len(tempos)):
    if tempos[i] >= tempo_referencia * 2:
        tamanhos_dobra.append(tamanhos[i])
        tempos_dobra.append(tempos[i])
        tempo_referencia = tempos[i]  # nova referência a partir daqui

# Gráfico de linha com Plotly
figura = go.Figure()

# Linha principal
figura.add_trace(go.Scatter(
    x=tamanhos,
    y=tempos,
    mode="lines+markers",
    name="Tempo medido",
    line=dict(color="#378ADD", width=2),
    marker=dict(size=6, color="#185FA5"),
))

# Marcadores de dobramento
figura.add_trace(go.Scatter(
    x=tamanhos_dobra,
    y=tempos_dobra,
    mode="markers",
    name="Tempo dobrou",
    marker=dict(
        symbol="star",
        size=16,
        color="#E8472A",
        line=dict(color="#9C2A12", width=1.5),
    ),
))

# Linhas verticais (shapes) em cada ponto de dobramento
shapes = []
for x in tamanhos_dobra:
    shapes.append(dict(
        type="line",
        x0=x, x1=x,
        y0=0, y1=1,
        yref="paper",          # atravessa o gráfico inteiro verticalmente
        line=dict(color="#E8472A", width=1.5, dash="dot"),
    ))

figura.update_layout(
    title="Crescimento do tempo de execução — Multiplicação de Matrizes (sequencial)",
    xaxis_title="Tamanho da matriz (N x N)",
    yaxis_title="Tempo de execução (segundos)",
    xaxis=dict(tickmode="array", tickvals=tamanhos),
    hovermode="x unified",
    template="plotly_white",
    font=dict(size=13),
    shapes=shapes,
)

figura.write_html("benchmarkGraficoImagem.html")
print("Gráfico salvo em: benchmarkGrafico.html")

if tamanhos_dobra:
    print(f"\nO tempo dobrou {len(tamanhos_dobra)} vez(es):")
    for x, t in zip(tamanhos_dobra, tempos_dobra):
        print(f"  N={x:>6}  →  {t:.4f}s")
else:
    print("\nO tempo não chegou a dobrar em nenhum ponto.")