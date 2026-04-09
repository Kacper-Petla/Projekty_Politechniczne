import java.util.*;

public abstract class Roslina extends Organizm
{
    public Roslina(int sila, int x, int y, Swiat swiat)
    {
        super(sila, Stale.INICJATYWA_ROSLIN, x, y, swiat, Stale.TypOrganizmu.ROSLINA);
    }

    @Override
    public void akcja()
    {
        wiek++;

        if (new Random().nextInt(100) < Stale.PRAWDOPODOBIENSTWO_ROZPRZESTRZENIANIA)
        {
            rozmnazanie();
        }
    }

    @Override
    public void kolizja(Organizm atakujacy)
    {
        swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ") zjedzony przez " + atakujacy.rysowanie() + " (" + atakujacy.getX() + ", " + atakujacy.getY() + ")");

        swiat.ustawOrganizmNa(atakujacy.getX(), atakujacy.getY(), null);
        atakujacy.setX(x);
        atakujacy.setY(y);
        swiat.ustawOrganizmNa(x, y, atakujacy);

        swiat.usunOrganizm(this);
    }

    public void rozmnazanie()
    {
        int[] XY = swiat.wylosujPustegoSasiada(this);
        if (XY != null)
        {
            Organizm potomek = stworzPotomka(XY[0], XY[1]);
            swiat.dodajOrganizm(potomek);
            swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ") rozprzestrzenił się na: " + XY[0] + ", " + XY[1]);
        }
        else
        {
            swiat.dodajLog(rysowanie() + " (" + x + ", " + y + ") brak miejsca do rozprzestrzenienia");
        }
    }
}