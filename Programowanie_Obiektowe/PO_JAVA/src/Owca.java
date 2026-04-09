public class Owca extends Zwierze
{
    public Owca(int x, int y, Swiat swiat)
    {
        super(Stale.OWCA_SILA, Stale.OWCA_INICJATYWA, x, y, swiat);
    }

    @Override
    public char rysowanie()
    {
        return Stale.OWCA;
    }

    @Override
    public String nazwa()
    {
        return Stale.OWCA_S;
    }

    @Override
    public Organizm stworzPotomka(int x, int y)
    {
        return new Owca(x, y, swiat);
    }
}