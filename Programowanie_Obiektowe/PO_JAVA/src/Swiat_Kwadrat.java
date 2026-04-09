import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Swiat_Kwadrat extends Swiat
{
    public Swiat_Kwadrat(int szerokosc, int wysokosc)
    {
        super(szerokosc, wysokosc, Stale.KWADRAT);
    }

    @Override
    protected void inicjalizujOkno()
    {
        okno = new JFrame("okno");
        okno.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        okno.setSize(Stale.PLANSZA_X, Stale.PLANSZA_Y);

        panel_planszy = new JPanel();
        panel_planszy.setLayout(new GridLayout(wysokosc, szerokosc));
        komorki = new JLabel[wysokosc][szerokosc];

        for (int y = wysokosc - 1; y >= 0; y--)
        {
            for (int x = 0; x < szerokosc; x++)
            {
                JLabel label = new JLabel(" ", SwingConstants.CENTER);
                label.setBorder(BorderFactory.createLineBorder(Color.GRAY));
                label.setFont(new Font("Monospaced", Font.BOLD, Stale.FONT_SIZE));
                komorki[y][x] = label;
                panel_planszy.add(label);
            }
        }

        panel_planszy.addComponentListener(new ComponentAdapter() {
            @Override
            public void componentResized(ComponentEvent e)
            {
                int width = panel_planszy.getWidth();
                int height = panel_planszy.getHeight();

                int tileWidth = width / szerokosc;
                int tileHeight = height / wysokosc;
                int tileSize = Math.min(tileWidth, tileHeight);

                int fontSize = tileSize / 2;

                for (int y = 0; y < wysokosc; y++)
                {
                    for (int x = 0; x < szerokosc; x++)
                    {
                        JLabel label = komorki[y][x];
                        label.setFont(new Font("Monospaced", Font.BOLD, fontSize));
                    }
                }
            }
        });

        okno.add(panel_planszy);

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
                    case KeyEvent.VK_Q: gx = -1; gy = 1; move = true; break;
                    case KeyEvent.VK_W: gx = 0; gy = 1; move = true; break;
                    case KeyEvent.VK_E: gx = 1; gy = 1; move = true; break;
                    case KeyEvent.VK_A: gx = -1; gy = 0; move = true; break;
                    case KeyEvent.VK_D: gx = 1; gy = 0; move = true; break;
                    case KeyEvent.VK_Z: gx = -1; gy = -1; move = true; break;
                    case KeyEvent.VK_S: gx = 0; gy = -1; move = true; break;
                    case KeyEvent.VK_C: gx = 1; gy = -1; move = true; break;
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

        panel_planszy.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                int szerokoscPola = panel_planszy.getWidth() / szerokosc;
                int wysokoscPola = panel_planszy.getHeight() / wysokosc;

                int x = e.getX() / szerokoscPola;
                int y = e.getY() / wysokoscPola;
                y = wysokosc - 1 - y;

                if (getOrganizmNa(x, y) == null)
                {
                    dodajNaPlansze(x, y);
                }
            }
        });
    }

    @Override
    public int[] wylosujSasiada(Organizm obecny)
    {
        int[] dx = { -1, 0, 1, -1, 1, -1, 0, 1 };
        int[] dy = { 1, 1, 1, 0, 0, -1, -1, -1 };

        java.util.List<Integer> indeksy = new ArrayList<>();
        for (int i = 0; i < 8; i++) indeksy.add(i);
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
        int[] dx = { -1, 0, 1, -1, 1, -1, 0, 1 };
        int[] dy = { 1, 1, 1, 0, 0, -1, -1, -1 };

        java.util.List<Integer> indeksy = new ArrayList<>();
        for (int i = 0; i < 8; i++) indeksy.add(i);
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
        int[] dx = { -1, 0, 1, -1, 1, -1, 0, 1 };
        int[] dy = {  1, 1, 1, 0, 0, -1, -1, -1 };

        List<int[]> sasiedzi = new ArrayList<>();
        for (int i = 0; i < 8; i++)
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
