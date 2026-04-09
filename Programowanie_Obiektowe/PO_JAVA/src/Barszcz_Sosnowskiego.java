import java.util.List;
import java.util.Random;

public class Barszcz_Sosnowskiego extends Roslina
{
    public Barszcz_Sosnowskiego(int x, int y, Swiat swiat)
    {
        super(Stale.BARSZCZ_SOSNOWSKIEGO_SILA, x, y, swiat);
    }

    @Override
    public void akcja()
    {
        wiek++;

        if (new Random().nextInt(100) < Stale.PRAWDOPODOBIENSTWO_ROZPRZESTRZENIANIA)
        {
            rozmnazanie();
        }

        List<int[]> sasiedzi = swiat.sasiedniePola(x, y);

        for (int[] pole : sasiedzi)
        {
            int newX = pole[0];
            int newY = pole[1];

            Organizm cel = swiat.getOrganizmNa(newX, newY);
            if (cel != null && cel.getTyp() == Stale.TypOrganizmu.ZWIERZE && cel.rysowanie() != Stale.CYBER_OWCA)
            {
                swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ") zabija " + cel.rysowanie() + " (" + cel.getX() + ", " + cel.getY() + ")");

                swiat.ustawOrganizmNa(cel.getX(), cel.getY(), null);
                swiat.usunOrganizm(cel);
            }
        }
    }

    @Override
    public void kolizja(Organizm atakujacy)
    {
        swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ") zjedzony przez " + atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ")");

        if (atakujacy.rysowanie() != Stale.CYBER_OWCA)
        {
            swiat.dodajLog(atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ") zatruty przez " + rysowanie());

            swiat.ustawOrganizmNa(atakujacy.getX(), atakujacy.getY(), null);
            swiat.usunOrganizm(atakujacy);
        }
        else
        {
            swiat.dodajLog(atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ") strawił " + rysowanie());
        }

        swiat.ustawOrganizmNa(x, y, null);
        swiat.usunOrganizm(this);
    }

    @Override
    public char rysowanie()
    {
        return Stale.BARSZCZ_SOSNOWSKIEGO;
    }

    @Override
    public String nazwa()
    {
        return Stale.BARSZCZ_SOSNOWSKIEGO_S;
    }

    @Override
    public Organizm stworzPotomka(int x, int y)
    {
        return new Barszcz_Sosnowskiego(x, y, swiat);
    }
}
