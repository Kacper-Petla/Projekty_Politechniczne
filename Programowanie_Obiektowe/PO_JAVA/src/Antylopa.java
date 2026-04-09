public class Antylopa extends Zwierze
{
    public Antylopa(int x, int y, Swiat swiat)
    {
        super(Stale.ANTYLOPA_SILA, Stale.ANTYLOPA_INICJATYWA, x, y, swiat);
    }

    @Override
    public void akcja()
    {
        wiek++;

        int tmp_x = x;
        int tmp_y = y;

        int[] XY = swiat.wylosujSasiada(this);
        if (XY != null)
        {
            x = XY[0];
            y = XY[1];

            while(true)
            {
                XY = swiat.wylosujSasiada(this);
                if (XY != null)
                {
                    if (XY[0] != tmp_x || XY[1] != tmp_y)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            if (XY != null)
            {
                x = tmp_x;
                y = tmp_y;
                przesun(XY[0], XY[1]);
            }
            else
            {
                x = tmp_x;
                y = tmp_y;
                swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ", " + sila + ") brak dostepnych ruchow");
            }
        }
        else
        {
            swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ", " + sila + ") brak dostepnych ruchow");
        }
    }

    @Override
    public void kolizja(Organizm atakujacy)
    {
        if (this.getClass() == atakujacy.getClass())
        {
            rozmnazanie();
        }
        else
        {
            if (new java.util.Random().nextBoolean())     // 50% szansy na ucieczkę
            {
                int[] XY = swiat.wylosujPustegoSasiada(this);
                if (XY != null)
                {
                    swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ", " + sila + ") uciekł przed " + atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ", " + atakujacy.getSila() + ") na pole: " + XY[0] + ", " + XY[1] + "\n");

                    swiat.ustawOrganizmNa(atakujacy.getX(), atakujacy.getY(), null);
                    atakujacy.setX(x);
                    atakujacy.setY(y);
                    swiat.ustawOrganizmNa(x, y, atakujacy);

                    x = XY[0];
                    y = XY[1];
                    swiat.ustawOrganizmNa(x, y, this);
                }
                else
                {
                    swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ", " + sila + ") nie zdołał uciec przed " + atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ", " + atakujacy.getSila() + ")");
                    swiat.usunOrganizm(this);
                }
            }
            atak(atakujacy);
        }
    }

    @Override
    public char rysowanie()
    {
        return Stale.ANTYLOPA;
    }

    @Override
    public String nazwa()
    {
        return Stale.ANTYLOPA_S;
    }

    @Override
    public Organizm stworzPotomka(int x, int y)
    {
        return new Antylopa(x, y, swiat);
    }
}