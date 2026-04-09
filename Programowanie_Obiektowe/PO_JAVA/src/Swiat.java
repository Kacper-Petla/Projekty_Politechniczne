import java.io.*;
import java.util.*;
import java.util.List;
import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public abstract class Swiat extends JFrame
{
    protected int szerokosc;
    protected int wysokosc;
    protected char typ_planszy;
    protected Organizm[][] plansza;
    protected List<Organizm> organizmy = new ArrayList<>();
    protected List<Organizm> organizmy_do_wykonania = new ArrayList<>();
    protected Organizm gracz;
    protected String log;

    protected JFrame okno;
    protected JPanel panel_planszy;
    protected JPanel panel_sterowania;
    protected JLabel[][] komorki;

    public Swiat(int szer, int wys, char typ)
    {
        this.szerokosc = szer;
        this.wysokosc = wys;
        this.typ_planszy = typ;
        this.plansza = new Organizm[wysokosc][szerokosc];
        log = "";
    }

    protected int[] wylosujPuste()
    {
        Random rand = new Random();

        int x = rand.nextInt(szerokosc);
        int y = rand.nextInt(wysokosc);

        while (getOrganizmNa(x, y) != null)
        {
            x = rand.nextInt(szerokosc);
            y = rand.nextInt(wysokosc);
        }

        return new int[] { x, y };
    }

    protected void zapiszGre()
    {
        String nazwa_pliku = JOptionPane.showInputDialog(okno, "Podaj nazwę pliku do zapisu:", "Zapis gry", JOptionPane.PLAIN_MESSAGE);

        if (nazwa_pliku == null || nazwa_pliku.trim().isEmpty())
        {
            JOptionPane.showMessageDialog(okno, "Anulowano zapis.");
            return;
        }

        nazwa_pliku = nazwa_pliku.trim() + ".txt";

        try (PrintWriter writer = new PrintWriter(new FileWriter(nazwa_pliku)))
        {
            writer.println(typ_planszy + " " + szerokosc + " " + wysokosc);

            if (gracz != null)
            {
                writer.println(
                                gracz.rysowanie() + " " +
                                gracz.getX() + " " +
                                gracz.getY() + " " +
                                gracz.getWiek() + " " +
                                gracz.getSila() + " " +
                                gracz.getTmp_Sila() + " " +
                                gracz.getUmiejetnosc()
                );
            }

            for (Organizm org : organizmy)
            {
                if (org != gracz)
                {
                    writer.println(
                                    org.rysowanie() + " " +
                                    org.getX() + " " +
                                    org.getY() + " " +
                                    org.getWiek() + " " +
                                    org.getSila()
                    );
                }
            }

            JOptionPane.showMessageDialog(okno, "Zapisano grę do pliku: " + nazwa_pliku);
        }
        catch (IOException e)
        {
            JOptionPane.showMessageDialog(okno, "Błąd zapisu: " + e.getMessage());
        }
    }

    protected boolean wykonajTure()
    {
        log = "";

        sortowanie();

        organizmy_do_wykonania = new ArrayList<>(organizmy);
        List<Organizm> tmp_organizmy = new ArrayList<>(organizmy);

        for (Organizm org : tmp_organizmy)
        {
            if (organizmy_do_wykonania.contains(org))
            {
                org.akcja();
            }
        }

        organizmyFix();

        if (gracz == null)
        {
            return false;
        }
        return true;
    }

    protected void sortowanie()
    {
        organizmy.sort((a, b) -> {
            if (a.getInicjatywa() != b.getInicjatywa())
            {
                return Integer.compare(b.getInicjatywa(), a.getInicjatywa());
            }
            return Integer.compare(b.getWiek(), a.getWiek());
        });
    }

    protected void organizmyFix()
    {
        for (Organizm org : organizmy)
        {
            if (getOrganizmNa(org.getX(), org.getY()) == null)
            {
                ustawOrganizmNa(org.getX(), org.getY(), org);
            }
        }
    }

    protected abstract void inicjalizujOkno();

    protected void aktywujUmiejetnosc()
    {
        int ile = Stale.CZAS_TRWANIA_UMIEJETNOSCI + Stale.CZAS_REGENERACJI_UMIEJETNOSCI + gracz.getUmiejetnosc() - gracz.getWiek();
        if (ile <= 0)
        {
            gracz.setUmiejetnosc(gracz.getWiek());
            JOptionPane.showMessageDialog(okno, "Aktywowano magiczny eliksir!");
        }
        else
        {
            JOptionPane.showMessageDialog(okno, "Umiejętność niedostępna przez: " + ile + " tur");
        }
    }

    protected abstract void inicjalizujInput();

    protected void dodajNaPlansze(int x, int y)
    {
        List<String> opcje = new ArrayList<>(Arrays.asList(
                Stale.WILK_S, Stale.OWCA_S, Stale.LIS_S, Stale.ZOLW_S, Stale.ANTYLOPA_S,
                Stale.TRAWA_S, Stale.MLECZ_S, Stale.GUARANA_S, Stale.WILCZA_JAGODA_S, Stale.BARSZCZ_SOSNOWSKIEGO_S
        ));

        //Naciśnięcie na wolne pole powinno dać możliwość dodania każdego z istniejących w świecie organizmów.
        /*
        List<String> symbole = new ArrayList<>(Arrays.asList(
                Stale.WILK_S, Stale.OWCA_S, Stale.LIS_S, Stale.ZOLW_S, Stale.ANTYLOPA_S,
                Stale.TRAWA_S, Stale.MLECZ_S, Stale.GUARANA_S, Stale.WILCZA_JAGODA_S, Stale.BARSZCZ_SOSNOWSKIEGO_S
        ));

        List<String> opcje = new ArrayList<>();

        for (Organizm org : organizmy)
        {
            if (symbole.contains(org.nazwa()))
            {
                symbole.remove(org.nazwa());
                opcje.add(org.nazwa());
            }
        }

        */

        String wybor = null;

        if (!opcje.isEmpty())
        {
            wybor = (String) JOptionPane.showInputDialog(okno, "Wybierz organizm do dodania:", "Dodaj organizm", JOptionPane.PLAIN_MESSAGE, null, opcje.toArray(), opcje.get(0));
        }
        else
        {
            JOptionPane.showMessageDialog(okno, "Brak organizmów do dodania.");
        }

        if (wybor != null)
        {
            Organizm nowy = null;
            switch (wybor)
            {
                case Stale.WILK_S: nowy = new Wilk(x, y, this); break;
                case Stale.OWCA_S: nowy = new Owca(x, y, this); break;
                case Stale.LIS_S: nowy = new Lis(x, y, this); break;
                case Stale.ZOLW_S: nowy = new Zolw(x, y, this); break;
                case Stale.ANTYLOPA_S: nowy = new Antylopa(x, y, this); break;
                case Stale.TRAWA_S: nowy = new Trawa(x, y, this); break;
                case Stale.MLECZ_S: nowy = new Mlecz(x, y, this); break;
                case Stale.GUARANA_S: nowy = new Guarana(x, y, this); break;
                case Stale.WILCZA_JAGODA_S: nowy = new Wilcza_Jagoda(x, y, this); break;
                case Stale.BARSZCZ_SOSNOWSKIEGO_S: nowy = new Barszcz_Sosnowskiego(x, y, this); break;
            }

            if (nowy != null)
            {
                dodajOrganizm(nowy);
                rysujSwiat();
            }
        }
    }

    protected void rysujSwiat()
    {
        okno.setTitle("Gra - tura " + gracz.getWiek());

        for (int y = wysokosc - 1; y >= 0; y--)
        {
            for (int x = 0; x < szerokosc; x++)
            {
                String symbol = " ";
                Color kolor = Color.LIGHT_GRAY;
                if (plansza[y][x] != null)
                {
                    symbol = plansza[y][x].rysowanie() + "";
                    Stale.TypOrganizmu typ = plansza[y][x].getTyp();

                    if (symbol.equals(Stale.CZLOWIEK + ""))
                    {
                        kolor = new Color(255, 200, 200);
                    }
                    else if (typ == Stale.TypOrganizmu.ZWIERZE)
                    {
                        kolor = new Color(200, 200, 255);
                    }
                    else if (typ == Stale.TypOrganizmu.ROSLINA)
                    {
                        kolor = new Color(200, 255, 200);
                    }
                }
                JLabel label = komorki[y][x];
                label.setText(symbol);
                label.setBackground(kolor);
                label.setOpaque(true);
            }
        }

        panel_planszy.revalidate();
        panel_planszy.repaint();
    }

    public abstract int[] wylosujSasiada(Organizm obecny);

    public abstract int[] wylosujPustegoSasiada(Organizm obecny);

    public abstract List<int[]> sasiedniePola(int x, int y);

    public void zainicjujGre()
    {
        int[] XY;
        XY = wylosujPuste();

        gracz = new Czlowiek(XY[0], XY[1], this);
        dodajOrganizm(gracz);

        int organizmNaPlansze = (int)((szerokosc * wysokosc) * Stale.PROCENT_PLANSZY / Stale.LICZBA_ORGANIZMOW);

        for (int i = 0; i < organizmNaPlansze; i++)
        {
            XY = wylosujPuste();
            dodajOrganizm(new Owca(XY[0], XY[1], this));
        }
        for (int i = 0; i < organizmNaPlansze; i++)
        {
            XY = wylosujPuste();
            dodajOrganizm(new Wilk(XY[0], XY[1], this));
        }
        for (int i = 0; i < organizmNaPlansze; i++)
        {
            XY = wylosujPuste();
            dodajOrganizm(new Lis(XY[0], XY[1], this));
        }
        for (int i = 0; i < organizmNaPlansze; i++)
        {
            XY = wylosujPuste();
            dodajOrganizm(new Zolw(XY[0], XY[1], this));
        }
        for (int i = 0; i < organizmNaPlansze; i++)
        {
            XY = wylosujPuste();
            dodajOrganizm(new Antylopa(XY[0], XY[1], this));
        }
        for (int i = 0; i < organizmNaPlansze; i++)
        {
            XY = wylosujPuste();
            dodajOrganizm(new Trawa(XY[0], XY[1], this));
        }
        for (int i = 0; i < organizmNaPlansze; i++)
        {
            XY = wylosujPuste();
            dodajOrganizm(new Mlecz(XY[0], XY[1], this));
        }
        for (int i = 0; i < organizmNaPlansze; i++)
        {
            XY = wylosujPuste();
            dodajOrganizm(new Guarana(XY[0], XY[1], this));
        }
        for (int i = 0; i < organizmNaPlansze; i++)
        {
            XY = wylosujPuste();
            dodajOrganizm(new Wilcza_Jagoda(XY[0], XY[1], this));
        }
        for (int i = 0; i < organizmNaPlansze; i++)
        {
            XY = wylosujPuste();
            dodajOrganizm(new Barszcz_Sosnowskiego(XY[0], XY[1], this));
        }
    }

    public void wczytajGre(BufferedReader reader) throws IOException
    {
        String line;
        while ((line = reader.readLine()) != null)
        {
            String[] tokens = line.split(" ");
            char symbol = tokens[0].charAt(0);
            int x = Integer.parseInt(tokens[1]);
            int y = Integer.parseInt(tokens[2]);
            int wiek = Integer.parseInt(tokens[3]);
            int sila = Integer.parseInt(tokens[4]);

            if (symbol == Stale.CZLOWIEK)
            {
                int tmp_sila = Integer.parseInt(tokens[5]);
                int umiejetnosc = Integer.parseInt(tokens[6]);
                gracz = new Czlowiek(x, y, this);
                gracz.setWiek(wiek);
                gracz.setSila(sila);
                gracz.setTmp_Sila(tmp_sila);
                gracz.setUmiejetnosc(umiejetnosc);
                dodajOrganizm(gracz);
            }
            else
            {
                Organizm org = null;

                switch (symbol)
                {
                    case Stale.OWCA -> org = new Owca(x, y, this);
                    case Stale.WILK -> org = new Wilk(x, y, this);
                    case Stale.LIS -> org = new Lis(x, y, this);
                    case Stale.ZOLW -> org = new Zolw(x, y, this);
                    case Stale.ANTYLOPA -> org = new Antylopa(x, y, this);
                    case Stale.TRAWA -> org = new Trawa(x, y, this);
                    case Stale.MLECZ -> org = new Mlecz(x, y, this);
                    case Stale.GUARANA -> org = new Guarana(x, y, this);
                    case Stale.WILCZA_JAGODA -> org = new Wilcza_Jagoda(x, y, this);
                    case Stale.BARSZCZ_SOSNOWSKIEGO -> org = new Barszcz_Sosnowskiego(x, y, this);
                }

                if (org != null)
                {
                    Organizm org2 = getOrganizmNa(x, y);
                    org.setWiek(wiek);
                    org.setSila(sila);
                    dodajOrganizm(org);
                    if (org2 != null && (symbol == Stale.TRAWA || symbol == Stale.MLECZ))
                    {
                        ustawOrganizmNa(x, y, org2);
                    }
                }
            }
        }
    }

    public void dodajOrganizm(Organizm org)
    {
        organizmy.add(org);
        plansza[org.getY()][org.getX()] = org;
    }

    public void rozpocznijGre()
    {
        inicjalizujOkno();
        inicjalizujInput();
        rysujSwiat();
    }

    public boolean czyNaPlanszy(int x, int y)
    {
        return x >= 0 && x < szerokosc && y >= 0 && y < wysokosc;
    }

    public void ustawOrganizmNa(int x, int y, Organizm org)
    {
        if (czyNaPlanszy(x, y))
        {
            plansza[y][x] = org;
        }
    }

    public void dodajLog(String tekst)
    {
        log += tekst + "\n";
    }

    public Organizm getOrganizmNa(int x, int y)
    {
        if (czyNaPlanszy(x, y))
        {
            return plansza[y][x];
        }
        return null;
    }

    public void usunOrganizm(Organizm org)
    {
        organizmy_do_wykonania.remove(org);

        if (organizmy.remove(org))
        {
            if (org == gracz)
            {
                gracz = null;
            }
        }
    }
}
