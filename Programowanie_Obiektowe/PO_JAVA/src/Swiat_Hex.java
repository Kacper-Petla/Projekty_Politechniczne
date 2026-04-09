import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Swiat_Hex extends Swiat
{
    public Swiat_Hex(int szerokosc, int wysokosc)
    {
        super(szerokosc, wysokosc, Stale.HEX);
    }

    @Override
    protected void inicjalizujOkno()
    {
        okno = new JFrame("okno");
        okno.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        okno.setLayout(new BorderLayout());

        panel_planszy = new JPanel(null);
        komorki = new JLabel[wysokosc][szerokosc];

        for (int y = 0; y < wysokosc; y++)
        {
            for (int x = 0; x < szerokosc; x++)
            {
                JLabel label = new JLabel(" ", SwingConstants.CENTER);
                label.setBorder(BorderFactory.createLineBorder(Color.GRAY));
                label.setFont(new Font("Monospaced", Font.BOLD, Stale.FONT_SIZE));
                label.setOpaque(true);
                label.setBackground(Color.WHITE);

                label.putClientProperty("x", x);
                label.putClientProperty("y", y);

                label.addMouseListener(new MouseAdapter() {
                    @Override
                    public void mouseClicked(MouseEvent e) {
                        JLabel clickedLabel = (JLabel) e.getSource();
                        int klikX = (int) clickedLabel.getClientProperty("x");
                        int klikY = (int) clickedLabel.getClientProperty("y");

                        if (getOrganizmNa(klikX, klikY) == null)
                        {
                            dodajNaPlansze(klikX, klikY);
                        }
                    }
                });

                komorki[y][x] = label;
                panel_planszy.add(label);
            }
        }

        panel_planszy.addComponentListener(new ComponentAdapter() {
            @Override
            public void componentResized(ComponentEvent e) {
                int width = panel_planszy.getWidth();
                int height = panel_planszy.getHeight();

                int tileSize = Math.min(width / (szerokosc + wysokosc) - 10, height / (szerokosc + wysokosc) * 2 - 10);
                int offsetX = width / 2;
                int offsetY = 20;

                for (int y = 0; y < wysokosc; y++)
                {
                    for (int x = 0; x < szerokosc; x++)
                    {
                        int px = offsetX + (x - y) * tileSize;
                        int py = offsetY + (x + y) * tileSize / 2;

                        JLabel label = komorki[y][x];
                        label.setBounds(px, py, tileSize, tileSize);
                        label.setFont(new Font("Monospaced", Font.BOLD, tileSize / 2));
                    }
                }

                panel_planszy.revalidate();
                panel_planszy.repaint();
            }
        });

        JScrollPane scrollPane = new JScrollPane(panel_planszy);
        okno.add(scrollPane, BorderLayout.CENTER);

        panel_sterowania = new JPanel();
        panel_sterowania.setLayout(new FlowLayout());

        JButton btnUmiejetnosc = new JButton("Umiejętność");
        btnUmiejetnosc.addActionListener(e -> {
            aktywujUmiejetnosc();
            okno.requestFocusInWindow();
        });

        JButton btnLogi = new JButton("Logi");
        btnLogi.addActionListener(e -> {
            JOptionPane.showMessageDialog(okno, "LOGI:\n" + log);
            okno.requestFocusInWindow();
        });

        JButton btnZapisz = new JButton("Zapisz");
        btnZapisz.addActionListener(e -> {
            zapiszGre();
            okno.requestFocusInWindow();
        });

        JButton btnWyjdz = new JButton("Wyjdź");
        btnWyjdz.addActionListener(e -> okno.dispose());

        panel_sterowania.add(btnUmiejetnosc);
        panel_sterowania.add(btnLogi);
        panel_sterowania.add(btnZapisz);
        panel_sterowania.add(btnWyjdz);

        okno.setFocusable(true);
        okno.requestFocusInWindow();
        okno.setVisible(true);

        okno.add(panel_sterowania, BorderLayout.NORTH);
        okno.pack();
        okno.setSize(Stale.PLANSZA_X, Stale.PLANSZA_Y);
        okno.setLocationRelativeTo(null);
        okno.setVisible(true);
    }

    @Override
    protected void inicjalizujInput()
    {
        okno.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                int gx = 0, gy = 0;
                boolean move = false;

                switch (e.getKeyCode())
                {
                    case KeyEvent.VK_Q: gx = -1; gy = 0; move = true; break;
                    case KeyEvent.VK_W: gx = -1; gy = -1; move = true; break;
                    case KeyEvent.VK_E: gx = 0; gy = -1; move = true; break;
                    case KeyEvent.VK_D: gx = 1; gy = 0; move = true; break;
                    case KeyEvent.VK_S: gx = 1; gy = 1; move = true; break;
                    case KeyEvent.VK_A: gx = 0; gy = 1; move = true; break;
                }
                if (move)
                {
                    int newX = gracz.getX() + gx;
                    int newY = gracz.getY() + gy;

                    if (czyNaPlanszy(newX, newY))
                    {
                        gracz.setGX(newX);
                        gracz.setGY(newY);
                        if (!wykonajTure())
                        {
                            JOptionPane.showMessageDialog(okno, "Koniec gry!");
                            okno.dispose();
                        }
                        else
                        {
                            rysujSwiat();
                        }
                    }
                    else
                    {
                        JOptionPane.showMessageDialog(okno, "Ruch poza plansze");
                    }
                }
            }
        });
    }

    @Override
    public int[] wylosujSasiada(Organizm obecny)
    {
        int[] dx = { -1, -1, 0, 1, 1, 0 };
        int[] dy = { 0, -1, -1, 0, 1, 1 };

        java.util.List<Integer> indeksy = new ArrayList<>();
        for (int i = 0; i < 6; i++) indeksy.add(i);
        Collections.shuffle(indeksy);

        for (int i : indeksy)
        {
            int newX = obecny.getX() + dx[i];
            int newY = obecny.getY() + dy[i];

            if (czyNaPlanszy(newX, newY))
            {
                return new int[] { newX, newY };
            }
        }
        return null;
    }

    @Override
    public int[] wylosujPustegoSasiada(Organizm obecny)
    {
        int[] dx = { -1, -1, 0, 1, 1, 0 };
        int[] dy = { 0, -1, -1, 0, 1, 1 };

        java.util.List<Integer> indeksy = new ArrayList<>();
        for (int i = 0; i < 6; i++) indeksy.add(i);
        Collections.shuffle(indeksy);

        for (int i : indeksy)
        {
            int newX = obecny.getX() + dx[i];
            int newY = obecny.getY() + dy[i];

            if (czyNaPlanszy(newX, newY) && getOrganizmNa(newX, newY) == null)
            {
                return new int[] { newX, newY };
            }
        }
        return null;
    }

    @Override
    public java.util.List<int[]> sasiedniePola(int x, int y)
    {
        int[] dx = { -1, -1, 0, 1, 1, 0 };
        int[] dy = { 0, -1, -1, 0, 1, 1 };

        List<int[]> sasiedzi = new ArrayList<>();
        for (int i = 0; i < 6; i++)
        {
            int newX = x + dx[i];
            int newY = y + dy[i];
            if (czyNaPlanszy(newX, newY))
            {
                sasiedzi.add(new int[]{newX, newY});
            }
        }
        return sasiedzi;
    }
}
