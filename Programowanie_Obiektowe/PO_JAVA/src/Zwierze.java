public abstract class Zwierze extends Organizm
{
    public Zwierze(int sila, int inicjatywa, int x, int y, Swiat swiat)
    {
        super(sila, inicjatywa, x, y, swiat, Stale.TypOrganizmu.ZWIERZE);
    }

    @Override
    public void akcja()
    {
        wiek++;

        int[] XY = swiat.wylosujSasiada(this);
        if (XY != null)
        {
            przesun(XY[0], XY[1]);
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
            atak(atakujacy);
        }
    }

    public void rozmnazanie()
    {
        int[] XY = swiat.wylosujPustegoSasiada(this);
        if (XY != null)
        {
            Organizm potomek = stworzPotomka(XY[0], XY[1]);
            swiat.dodajOrganizm(potomek);
            swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ", " + sila + ") wydał potomka " + rysowanie() + " (" + XY[0] + ", " + XY[1] + ", " + potomek.getSila() + ")");

        }
        else
        {
            swiat.dodajLog("Potomek " + rysowanie() + " (" + x + ", " + y + ", " + sila + ") został zgnieciony");
        }
    }

    public void przesun(int newX, int newY)
    {
        Organizm cel = swiat.getOrganizmNa(newX, newY);

        if (cel == null)
        {
            swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ", " + sila + ") przesuwa się na: " + newX + ", " + newY);
            swiat.ustawOrganizmNa(x, y, null);
            x = newX;
            y = newY;
            swiat.ustawOrganizmNa(x, y, this);
        }
        else
        {
            cel.kolizja(this);
        }
    }

    public void atak(Organizm atakujacy)
    {
        if (atakujacy.getSila() >= sila)
        {
            swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ", " + sila + ") zabity przez " + atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ", " + atakujacy.getSila() + ")");
            swiat.ustawOrganizmNa(atakujacy.getX(), atakujacy.getY(), null);
            atakujacy.setX(x);
            atakujacy.setY(y);
            swiat.ustawOrganizmNa(x, y, atakujacy);

            swiat.usunOrganizm(this);
        }
        else
        {
            swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ", " + sila + ") obronił się przed " + atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ", " + atakujacy.getSila() + ")");
            swiat.ustawOrganizmNa(atakujacy.getX(), atakujacy.getY(), null);
            swiat.usunOrganizm(atakujacy);
        }
    }

    public abstract Organizm stworzPotomka(int newX, int newY);
}
