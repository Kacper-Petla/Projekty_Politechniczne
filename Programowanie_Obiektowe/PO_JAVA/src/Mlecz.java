import java.util.Random;

public class Mlecz extends Roslina
{
    public Mlecz(int x, int y, Swiat swiat)
    {
        super(Stale.MLECZ_SILA, x, y, swiat);
    }

    @Override
    public void akcja()
    {
        wiek++;

        for (int i = 0; i < Stale.MLECZ_PROBY_ROZPRZESTRZENIENIA; i++)
        {
            if (new Random().nextInt(100) < Stale.PRAWDOPODOBIENSTWO_ROZPRZESTRZENIANIA)
            {
                rozmnazanie();
            }
        }
    }

    @Override
    public void kolizja(Organizm atakujacy)
    {
        swiat.ustawOrganizmNa(atakujacy.getX(), atakujacy.getY(), null);
        atakujacy.setX(x);
        atakujacy.setY(y);
        swiat.ustawOrganizmNa(x, y, atakujacy);

        if (atakujacy.rysowanie() == Stale.OWCA)
        {
            swiat.dodajLog(rysowanie() + " zjedzony przez " + atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ")");
            swiat.usunOrganizm(this);
        }
        else
        {
            swiat.dodajLog(rysowanie() + " pod " + atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ")");
        }
    }

    @Override
    public char rysowanie()
    {
        return Stale.MLECZ;
    }

    @Override
    public String nazwa()
    {
        return Stale.MLECZ_S;
    }

    @Override
    public Organizm stworzPotomka(int x, int y)
    {
        return new Mlecz(x, y, swiat);
    }
}